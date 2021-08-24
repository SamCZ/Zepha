//
// Created by aurailus on 11/06/19.
//

#include <gzip/compress.hpp>

#include "ServerLuaParser.h"

#include "ErrorFormatter.h"
#include "world/ServerWorld.h"
#include "util/net/Serializer.h"
#include "world/player/ServerPlayer.h"
#include "lua/register/RegisterItem.h"
#include "lua/register/RegisterBiome.h"
#include "lua/register/RegisterBlock.h"

// Usertypes
#include "lua/usertype/Target.h"
#include "lua/usertype/Player.h"
#include "lua/usertype/Entity.h"
#include "lua/usertype/Inventory.h"
#include "lua/usertype/Dimension.h"
#include "lua/usertype/ItemStack.h"
#include "lua/usertype/InventoryList.h"
#include "lua/usertype/AnimationManager.h"

// Modules
#include "lua/modules/Time.h"
#include "lua/modules/Message.h"
#include "lua/modules/Dimension.h"
#include "lua/modules/Structure.h"

// Util
#include "lua/register/CreateRegister.h"

ServerLuaParser::ServerLuaParser(ServerSubgame& game) : LuaParser(game) {}

void ServerLuaParser::init(WorldPtr world, const std::string& path) {
	lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table, sol::lib::debug);
	
	loadApi(world);
	handler.loadMods(static_cast<ServerSubgame&>(game), path + "mods");
	handler.executeMods(std::bind(&ServerLuaParser::runFileSandboxed, this, std::placeholders::_1));
	
	std::cout << Log::info << "Loaded " << handler.cGetMods().size() << " mods: [ ";
	for (unsigned int i = 0; i < handler.cGetMods().size(); i++)
		std::cout << handler.cGetMods()[i].config.name << (i < handler.cGetMods().size() - 1 ? ", " : " ]\n");
}

void ServerLuaParser::update(double delta) {
	accumulatedDelta += delta;
	while (accumulatedDelta > static_cast<double>(UPDATE_STEP)) {
		safe_function(core["__builtin"]["update_entities"], static_cast<double>(UPDATE_STEP));
		safe_function(core["__builtin"]["update_delayed_functions"]);
		accumulatedDelta -= static_cast<double>(UPDATE_STEP);
	}
}

void ServerLuaParser::sendModsPacket(ENetPeer* peer) const {
	for (const LuaMod& mod : handler.cGetMods())
		Serializer().append(mod.serialized).packet(Packet::Type::MODS).sendTo(peer, Packet::Channel::CONNECT);
	
	vec<string> order {};
	for (const LuaMod& mod : handler.cGetMods()) order.push_back(mod.config.name);
	Serializer().append(order).packet(Packet::Type::MOD_ORDER).sendTo(peer, Packet::Channel::CONNECT);
}

void ServerLuaParser::playerConnected(std::shared_ptr<ServerPlayer> client) {
	auto players = core.get<sol::table>("players");
	players.add(Api::Usertype::ServerPlayer(std::static_pointer_cast<Player>(client)));
	
	sol::object player = players[players.size()];
	
	safe_function(core["trigger"], "new_player", player);
	safe_function(core["trigger"], "player_join", player);
}

void ServerLuaParser::playerDisconnected(std::shared_ptr<ServerPlayer> player) {
	for (auto& pair : core.get<sol::table>("players")) {
		auto& p = pair.second.as<Api::Usertype::ServerPlayer>();
		if (p.get_id() == player->getId()) {
			safe_function(core["trigger"], "player_leave", p);
			
			core.get<sol::table>("players")[pair.first] = sol::nil;
			break;
		}
	}
}

void ServerLuaParser::loadApi(WorldPtr world) {
	//Create Zepha Table
	core = lua.create_table();
	lua["zepha"] = core;
	core["__builtin"] = lua.create_table();
	
	// Types
	Api::Usertype::Target::bind(Api::State::SERVER, lua, core);
	Api::Usertype::Entity::bind(Api::State::SERVER, lua, core);
	Api::Usertype::Inventory::bind(Api::State::SERVER, lua, core);
	Api::Usertype::Dimension::bind(Api::State::SERVER, lua, core);
	Api::Usertype::ItemStack::bind(Api::State::SERVER, lua, core);
	Api::Usertype::ServerPlayer::bind(Api::State::SERVER, lua, core);
	Api::Usertype::InventoryList::bind(Api::State::SERVER, lua, core);
	Api::Usertype::ServerAnimationManager::bind(Api::State::SERVER, lua, core);
	
	core["server"] = true;
	core["players"] = lua.create_table();
	
	// Modules
	modules.emplace_back(make_unique<Api::Module::Time>(Api::State::SERVER, lua, core));
	modules.emplace_back(make_unique<Api::Module::Message>(Api::State::SERVER, core, game, *world.s()));
	modules.emplace_back(make_unique<Api::Module::Dimension>(Api::State::SERVER, core, game, *world.s()));
	modules.emplace_back(make_unique<Api::Module::Structure>(Api::State::SERVER, core, game, *world.s()));
	
	// Register
	auto& game = static_cast<ServerSubgame&>(this->game);
	
	Api::Util::createRegister(lua, core, "mesh");
	Api::Util::createRegister(lua, core, "item",
		[&](const auto& iden) { RegisterItem::server(core, game, iden); });
	Api::Util::createRegister(lua, core, "block",
		[&](const auto& iden) { RegisterBlock::server(core, game, iden); });
	Api::Util::createRegister(lua, core, "biome",
		[&](const auto& iden) { RegisterBiome::server(core, game, iden); });
	Api::Util::createRegister(lua, core, "keybind");
	Api::Util::createRegister(lua, core, "blockmodel");
	Api::Util::createRegister(lua, core, "entity", nullptr, "entities");
	
	// Define keybind variables
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
	
	lua.set_function("require", &ServerLuaParser::runFileSandboxed, this);
	lua["dofile"] = lua["loadfile"] = lua["require"];
}

sol::protected_function_result ServerLuaParser::runFileSandboxed(const std::string& file) {
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
			
			using Pfr = sol::protected_function_result;
			return lua.safe_script(f.file, env,
				[](lua_State*, Pfr pfr) -> Pfr { throw static_cast<sol::error>(pfr); },
				"@" + f.path, sol::load_mode::text);
		}
		throw std::runtime_error("Error opening \"" + file + "\", file not found.");
	}
	throw std::runtime_error("Error opening \"" + file + "\", mod not found.");
}