//
// Created by aurailus on 17/12/18.
//

#include "LocalLuaParser.h"

#include "client/Client.h"
#include "lua/ModException.h"
#include "lua/ErrorFormatter.h"
#include "lua/register/RegisterItem.h"
#include "lua/register/RegisterBlock.h"
#include "lua/register/RegisterBiome.h"
#include "lua/register/RegisterKeybind.h"

// Usertypes
#include "lua/usertype/Target.h"
#include "lua/usertype/Player.h"
#include "lua/usertype/Entity.h"
#include "lua/usertype/Inventory.h"
#include "lua/usertype/Dimension.h"
#include "lua/usertype/ItemStack.h"
#include "lua/usertype/GuiElement.h"
#include "lua/usertype/KeyObserver.h"
#include "lua/usertype/InventoryList.h"
#include "lua/usertype/AnimationManager.h"

// Modules
#include "lua/modules/Time.h"
#include "lua/modules/Message.h"
#include "lua/modules/Dimension.h"
#include "lua/modules/Structure.h"

// Util
#include "lua/register/CreateRegister.h"

LocalLuaParser::LocalLuaParser(LocalSubgame& game) : LuaParser(game) {}

void LocalLuaParser::init(WorldPtr world, PlayerPtr player, Client& client) {
	this->client = &client;
	this->player = player;
	lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug);
	
	loadApi(world);
	
	try {
		for (let& mod : modOrder) loadFile(mods[mod].clientMain);
	}
	catch (sol::error r) {
		string err = r.what();
		std::cout << Log::err << err << Log::endl;
	
		vec<string> lines;
		{
			string line;
			std::stringstream textStream(err);
			while (std::getline(textStream, line, '\n')) lines.emplace_back(line);
		}
	
		for (const let& line : lines) {
			usize fileNameStart = line.find('/');
			if (fileNameStart == string::npos) continue;
			usize lineNumStart = line.find(':', fileNameStart + 1);
			if (lineNumStart == string::npos) continue;
			usize lineNumEnd = line.find(':', lineNumStart + 1);
			if (lineNumEnd == string::npos) continue;
	
			string fullPath = line.substr(0, lineNumStart);
			fullPath.erase(std::remove_if(fullPath.begin(), fullPath.end(), isspace), fullPath.end());
			
			fileNameStart = fullPath.find('/');
			string modName = fullPath.substr(0, fileNameStart);
			if (modName == "base") continue;
			
			let modIter = mods.find(modName);
			if (modIter == mods.end()) continue;
			let& mod = modIter->second;
			
			let fileIter = mod.files.find(fullPath);
			if (fileIter == mod.files.end()) continue;
			
			let msg = ErrorFormatter::formatError(fullPath,
				std::stoi(line.substr(lineNumStart + 1, lineNumEnd - lineNumStart - 1)),
				err, fileIter->first);
			
			throw ModException(msg);
		}
	
		throw ModException(err);
	}
}

void LocalLuaParser::update(double delta) {
	accumulatedDelta += delta;
	while (accumulatedDelta > static_cast<double>(UPDATE_STEP)) {
		safe_function(core["__builtin"]["update_entities"], static_cast<double>(UPDATE_STEP));
		safe_function(core["__builtin"]["update_delayed_functions"]);
		accumulatedDelta -= static_cast<double>(UPDATE_STEP);
	}
}

void LocalLuaParser::addMod(const Mod& mod) {
	mods.emplace(mod.identifier, mod);
}

void LocalLuaParser::setModLoadOrder(const vec<string> order) {
	modOrder = order;
}

void LocalLuaParser::addKBObserver(usize id) {
	kbObservers.emplace(id);
	player.l()->setKBIndicatorVisible(true);
}

void LocalLuaParser::removeKBObserver(usize id) {
	kbObservers.erase(id);
	player.l()->setKBIndicatorVisible(kbObservers.size());
}

void LocalLuaParser::loadApi(WorldPtr world) {
	//Create Zepha Table
	core = lua.create_table();
	lua["zepha"] = core;
	core["__builtin"] = lua.create_table();
	
	// Types
	Api::Usertype::Target::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::Entity::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::Inventory::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::Dimension::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::ItemStack::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::LocalPlayer::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::GuiElement::bind(lua, core, player.l()->getRoot());
	Api::Usertype::InventoryList::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::LocalAnimationManager::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::KeyObserver::bind(lua, core, client->renderer.window.input, *this);
	
	core["client"] = true;
	core["player"] = Api::Usertype::LocalPlayer(player);
	
	// Modules
	modules.emplace_back(std::make_unique<Api::Module::Time>(Api::State::CLIENT, lua, core));
	modules.emplace_back(std::make_unique<Api::Module::Message>(Api::State::CLIENT, core, game, **world));
	modules.emplace_back(std::make_unique<Api::Module::Dimension>(Api::State::CLIENT, core, game, **world));
	modules.emplace_back(std::make_unique<Api::Module::Structure>(Api::State::CLIENT, core, game, **world));
	
	// Register
	auto& game = static_cast<LocalSubgame&>(this->game);
	
	Api::Util::createRegister(lua, core, "mesh");
	Api::Util::createRegister(lua, core, "item",
		[&](const string& iden) { RegisterItem::client(core, game, iden); });
	Api::Util::createRegister(lua, core, "block",
		[&](const string& iden) { RegisterBlock::client(core, game, iden); });
	Api::Util::createRegister(lua, core, "biome",
		[&](const string& iden) { RegisterBiome::client(core, game, iden); });
	Api::Util::createRegister(lua, core, "keybind",
		[&](const string& iden) { RegisterKeybind::client(core, client->renderer.window.input, refs, iden); });
	Api::Util::createRegister(lua, core, "blockmodel");
	Api::Util::createRegister(lua, core, "entity", nullptr, "entities");
	
	// Keybind Variables
	core["keys"] = lua.create_table();
	core["keycodes"] = lua.create_table();
	
	for (unsigned short i = 0; i < 350; i++) {
		auto key = ::Util::getKeyStr(i);
		if (!key.empty()) {
			core["keys"][key] = i;
			core["keycodes"][i] = key;
		}
	}
	
	bindModules();
	
	lua.set_function("require", &LocalLuaParser::require, this);
	lua["dofile"] = lua["loadfile"] = lua["require"];
}

//sol::protected_function_result LocalLuaParser::require(const string& path) {
//	usize modnameEnd = path.find('/');
//	if (modnameEnd == string::npos) throw std::runtime_error(
//		"Error opening '" + path + "', specified file is invalid.");
//	string modname = path.substr(0, modnameEnd);
//
//	let modIt = mods.find(modname);
//	if (modIt == mods.end()) throw sol::error("Error opening '" + path + "', mod not found.");
//	let& mod = modIt->second;
//
//	let fileIt = mod.files.find(path);
//	if (fileIt == mod.files.end()) throw sol::error("Error opening '" + path + "', file not found.");
//	let& file = fileIt->second;
//
//	sol::environment env(lua, sol::create, lua.globals());
//	env["_PATH"] = path.substr(0, path.find_last_of('/') + 1);
//	env["_FILE"] = path;
//	env["_MODNAME"] = mod.identifier;
//
//	using Pfr = sol::protected_function_result;
//	return lua.safe_script(file, env,
//		[](lua_State*, Pfr pfr) -> Pfr { throw static_cast<sol::error>(pfr); },
//		"@" + path, sol::load_mode::text);
//}

sol::protected_function_result LocalLuaParser::require(sol::this_environment env, string requirePath) {
	string currentPath = static_cast<sol::environment>(env).get<string>("__PATH");
	vec<string> pathSegments;
	
	if (requirePath[0] == '.') {
		usize lastPos = 0, pos;
		while ((pos = currentPath.find('/', lastPos)) != string::npos) {
			pathSegments.emplace_back(currentPath.substr(lastPos, pos - lastPos));
			lastPos = pos + 1;
		}
		if (lastPos < currentPath.size())
			pathSegments.emplace_back(currentPath.substr(lastPos, currentPath.size()));
		
		lastPos = 0;
		while ((pos = requirePath.find('/', lastPos)) != string::npos) {
			pathSegments.emplace_back(requirePath.substr(lastPos, pos - lastPos));
			lastPos = pos + 1;
		}
		if (lastPos < requirePath.size())
			pathSegments.emplace_back(requirePath.substr(lastPos, requirePath.size()));
		
		for (let it = pathSegments.begin(); it != pathSegments.end();) {
			if (*it == ".") {
				pathSegments.erase(it);
			}
			else if (*it == "..") {
				if (it - pathSegments.begin() < 2) throw std::runtime_error("Path escapes sandbox.");
				pathSegments.erase(it);
				pathSegments.erase(--it);
			}
			else it++;
		}
		
		std::stringstream s;
		for (usize i = 0; i < pathSegments.size(); i++) s << (i != 0 ? "/" : "") << pathSegments[i];
		requirePath = s.str();
	}
	
	return loadFile(requirePath);
}

sol::protected_function_result LocalLuaParser::loadFile(string path) {
	usize modnameEnd = path.find('/');
	if (modnameEnd == string::npos) throw std::runtime_error(
		"Error opening '" + path + "', specified file is invalid.");
	string modname = path.substr(0, modnameEnd);
	
	let modIt = mods.find(modname);
	if (modIt == mods.end()) throw sol::error("Error opening '" + path + "', mod not found.");
	let& mod = modIt->second;
	
	std::string_view file;
	let it = mod.files.find(path);
	if (it != mod.files.end()) file = it->second;
	else {
		it = mod.files.find(path + "/main");
		if (it == mod.files.end()) throw sol::error("Error opening '" + path + "', file not found.");
		file = it->second;
		path += "/main";
	}
	
	sol::environment env(lua, sol::create, lua.globals());
	env["__PATH"] = path.substr(0, path.find_last_of('/') + 1);
	env["__FILE"] = path;
	env["__MOD_NAME"] = mod.identifier;
	
	using Pfr = sol::protected_function_result;
	return lua.safe_script(file, env,
		[](lua_State*, Pfr pfr) -> Pfr { throw static_cast<sol::error>(pfr); },
		"@" + path, sol::load_mode::text);
}