//
// Created by aurailus on 17/12/18.
//

#include "LocalLuaParser.h"

#include "client/Client.h"
#include "ErrorFormatter.h"
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
	lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug);
	
	loadApi(world, player);
	handler.executeMods(Util::bind_this(this, &LocalLuaParser::runFileSandboxed));
	client.renderer.window.input.setCallback(Util::bind_this(&keybinds, &LuaKeybindHandler::keybindHandler));
}

void LocalLuaParser::update(double delta) {
	accumulatedDelta += delta;
	while (accumulatedDelta > static_cast<double>(UPDATE_STEP)) {
		safe_function(core["__builtin"]["update_entities"], static_cast<double>(UPDATE_STEP));
		safe_function(core["__builtin"]["update_delayed_functions"]);
		accumulatedDelta -= static_cast<double>(UPDATE_STEP);
	}
}

LocalModHandler& LocalLuaParser::getHandler() {
	return handler;
}

void LocalLuaParser::loadApi(WorldPtr world, PlayerPtr player) {
	//Create Zepha Table
	core = lua.create_table();
	lua["zepha"] = core;
	core["__builtin"] = lua.create_table();
	
	// Types
	ClientApi::gui_element(lua);
	
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
	
	// Create sandboxed runfile()
	lua["dofile"] = lua["loadfile"] = sol::nil;
	lua.set_function("runfile", &LocalLuaParser::runFileSandboxed, this);
}

sol::protected_function_result LocalLuaParser::errorCallback(sol::protected_function_result r) const {
	sol::error err = r;
	std::string errString = err.what();
	
	try {
		std::string::size_type slash = errString.find('/');
		if (slash != std::string::npos) throw "npos";
		
		std::string modString = errString.substr(0, slash);
		
		std::string::size_type lineNumStart = errString.find(':', slash);
		if (lineNumStart != std::string::npos) throw "lineNumStart";
		std::string::size_type lineNumEnd = errString.find(':', lineNumStart + 1);
		if (lineNumEnd != std::string::npos) throw "lineNumEnd";
		
		std::string fileName = errString.substr(0, lineNumStart);
		int lineNum = std::stoi(errString.substr(lineNumStart + 1, lineNumEnd - lineNumStart - 1));
		
		for (const auto& mod : handler.cGetMods()) {
			if (mod.config.name == modString) {
				for (auto& file : mod.files) {
					if (file.path == fileName) {
						std::cout << std::endl << ErrorFormatter::formatError(fileName, lineNum, errString, file.file)
						          << std::endl;
						break;
					}
				}
				break;
			}
		}
	}
	catch (...) {
		std::cout << Log::err << "Zepha has encountered an error, and ErrorFormatter failed to format it:"
		          << std::endl << std::endl << errString << Log::endl;
	}
	
	throw std::runtime_error("Exiting.");
}

sol::protected_function_result LocalLuaParser::runFileSandboxed(const std::string& file) {
	size_t modname_length = file.find('/');
	if (modname_length == std::string::npos)
		throw std::runtime_error("Error opening \"" + file + "\", specified file is invalid.");
	std::string modname = file.substr(0, modname_length);
	
	for (const LuaMod& mod : handler.cGetMods()) {
		if (modname != mod.config.name) continue;
		for (const LuaMod::File& f : mod.files) {
			if (f.path != file) continue;
			
			sol::environment env(lua, sol::create, lua.globals());
			env["_PATH"] = f.path.substr(0, f.path.find_last_of('/') + 1);
			env["_FILE"] = f.path;
			env["_MODNAME"] = mod.config.name;
			
			return lua.safe_script(f.file, env, std::bind(&LocalLuaParser::errorCallback, this, std::placeholders::_2),
				"@" + f.path, sol::load_mode::text);
		}
		throw std::runtime_error("Error opening \"" + file + "\", file not found.");
	}
	throw std::runtime_error("Error opening \"" + file + "\", mod not found.");
}