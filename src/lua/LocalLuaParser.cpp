//
// Created by aurailus on 17/12/18.
//

#include "LocalLuaParser.h"

#include "client/Client.h"
#include "ErrorFormatter.h"
#include "lua/ModException.h"
#include "register/RegisterItem.h"
#include "register/RegisterBlock.h"
#include "register/RegisterBiome.h"
#include "register/RegisterKeybind.h"

// Usertypes
#include "usertype/Target.h"
#include "usertype/Player.h"
#include "usertype/Entity.h"
#include "usertype/Inventory.h"
#include "usertype/Dimension.h"
#include "usertype/ItemStack.h"
#include "usertype/InventoryList.h"
#include "usertype/AnimationManager.h"

#include "usertype/LuaGuiElement.h"

// Modules
#include "modules/Time.h"
#include "modules/Message.h"
#include "modules/Dimension.h"
#include "modules/Structure.h"

// Util
#include "lua/register/CreateRegister.h"

LocalLuaParser::LocalLuaParser(LocalSubgame& game) : LuaParser(game), keybinds(this) {}

void LocalLuaParser::init(WorldPtr world, PlayerPtr player, Client& client) {
	this->client = &client;
	lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug);
	
	loadApi(world, player);
	
	try {
		runFileSandboxed("base/init");
		for (string mod : modLoadOrder) if (mod != "base") runFileSandboxed(mod + "/init");
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
			usize lineNumStart = line.find(':');
			if (lineNumStart == string::npos) continue;
			usize lineNumEnd = line.find(':', lineNumStart + 1);
			if (lineNumEnd == string::npos) continue;
	
			string fullPath = line.substr(0, lineNumStart);
			fullPath.erase(std::remove_if(fullPath.begin(), fullPath.end(), isspace), fullPath.end());
	
			usize slashPos = fullPath.find('/');
			if (slashPos == string::npos) continue;
			string modName = fullPath.substr(0, slashPos);
			if (modName == "base") continue;
			
			let iter = mods.find(modName);
			if (iter == mods.end()) continue;
			
			for (const let& file : iter->second.files) {
				if (file.path != fullPath) continue;
	
				let msg = ErrorFormatter::formatError(fullPath,
					std::stoi(line.substr(lineNumStart + 1, lineNumEnd - lineNumStart - 1)),
					err, file.file);
				
				throw ModException(msg);
			}
		}
	
		throw ModException(err);
	}
	
//	client.renderer.window.input.setCallback(Util::bind_this(&keybinds, &LuaKeybindHandler::keybindHandler));
}

void LocalLuaParser::update(double delta) {
	accumulatedDelta += delta;
	while (accumulatedDelta > static_cast<double>(UPDATE_STEP)) {
		safe_function(core["__builtin"]["update_entities"], static_cast<double>(UPDATE_STEP));
		safe_function(core["__builtin"]["update_delayed_functions"]);
		accumulatedDelta -= static_cast<double>(UPDATE_STEP);
	}
}

void LocalLuaParser::addMod(const LuaMod& mod) {
	mods.emplace(mod.config.name, mod);
}

void LocalLuaParser::setModLoadOrder(const vec<string> order) {
	modLoadOrder = order;
}

void LocalLuaParser::loadApi(WorldPtr world, PlayerPtr player) {
	//Create Zepha Table
	core = lua.create_table();
	lua["zepha"] = core;
	core["__builtin"] = lua.create_table();
	
	// Types
	Api::Usertype::GuiElement::bind(lua, core, player.l()->getRoot());
	Api::Usertype::Target::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::Entity::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::Inventory::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::Dimension::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::ItemStack::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::LocalPlayer::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::InventoryList::bind(Api::State::CLIENT, lua, core);
	Api::Usertype::LocalAnimationManager::bind(Api::State::CLIENT, lua, core);
	
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
		[&](const auto& iden) { RegisterItem::client(core, game, iden); });
	Api::Util::createRegister(lua, core, "block",
		[&](const auto& iden) { RegisterBlock::client(core, game, iden); });
	Api::Util::createRegister(lua, core, "biome",
		[&](const auto& iden) { RegisterBiome::client(core, game, iden); });
	Api::Util::createRegister(lua, core, "keybind",
		[&](const auto& iden) { RegisterKeybind::client(core, keybinds, iden); });
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
	
	lua.set_function("require", &LocalLuaParser::runFileSandboxed, this);
	lua["dofile"] = lua["loadfile"] = lua["require"];
}

sol::protected_function_result LocalLuaParser::errorCallback(sol::protected_function_result r) {
}

sol::protected_function_result LocalLuaParser::runFileSandboxed(const std::string& file) {
	size_t modname_length = file.find('/');
	if (modname_length == std::string::npos)
		throw std::runtime_error("Error opening \"" + file + "\", specified file is invalid.");
	std::string modname = file.substr(0, modname_length);
	
	let iter = mods.find(modname);
	if (iter == mods.end()) throw std::runtime_error("Error opening \"" + file + "\", mod not found.");
	
	for (const LuaMod::File& f : iter->second.files) {
		if (f.path != file) continue;
		
		sol::environment env(lua, sol::create, lua.globals());
		env["_PATH"] = f.path.substr(0, f.path.find_last_of('/') + 1);
		env["_FILE"] = f.path;
		env["_MODNAME"] = modname;
		
		using Pfr = sol::protected_function_result;
		let res = lua.safe_script(f.file, env,
			[](lua_State*, Pfr pfr) -> Pfr { throw static_cast<sol::error>(pfr); },
			"@" + f.path, sol::load_mode::text);
		return res;
	}
	throw std::runtime_error("Error opening \"" + file + "\", file not found.");
}
