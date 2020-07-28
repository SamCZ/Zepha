//
// Created by aurailus on 11/06/19.
//

#include <gzip/compress.hpp>
#include <enet/enet.h>

#include "ServerLuaParser.h"

#include "ErrorFormatter.h"
#include "../net/Serializer.h"
#include "register/RegisterItems.h"
#include "register/RegisterBiomes.h"
#include "register/RegisterBlocks.h"
#include "../net/server/world/ServerWorld.h"

// Usertypes
#include "usertype/ServerLuaPlayer.h"
#include "usertype/sLuaEntity.h"
#include "usertype/sInventoryRef.h"
#include "usertype/cItemStack.h"
#include "usertype/Target.h"

// Modules
#include "modules/Time.h"
#include "modules/Block.h"
#include "modules/Entity.h"
#include "modules/Register.h"

#include "modules/create_structure.h"
#include "LuaMod.h"

ServerLuaParser::ServerLuaParser(ServerSubgame& game) : LuaParser(game), game(game) {}

void ServerLuaParser::init(ServerWorld& world, const std::string& path) {
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);

    loadApi(game, world);
    handler.loadMods(game, path + "mods");
    handler.executeMods(std::bind(&ServerLuaParser::runFileSandboxed, this, std::placeholders::_1));

    registerDefs(game);

    std::cout << Log::info << "Loaded " << handler.cGetMods().size() << " mods: [ ";
    for (unsigned int i = 0; i < handler.cGetMods().size(); i++)
        std::cout << handler.cGetMods()[i].config.name << (i < handler.cGetMods().size() - 1 ? ", " : " ]\n");
}

void ServerLuaParser::update(double delta) {
    this->delta += delta;
    while (this->delta > static_cast<double>(UPDATE_STEP)) {
        safe_function(core["__builtin"]["update_entities"], static_cast<double>(UPDATE_STEP));
        safe_function(core["__builtin"]["update_delayed_functions"]);
        this->delta -= static_cast<double>(UPDATE_STEP);
    }
}

void ServerLuaParser::sendModsPacket(ENetPeer* peer) const {
    for (const LuaMod& mod : handler.cGetMods())
        Serializer().append(mod.serialized).packet(PacketType::MODS).sendTo(peer, PacketChannel::CONNECT);

    std::vector<std::string> order {};
    for (const LuaMod& mod : handler.cGetMods()) order.push_back(mod.config.name);
    Serializer().append(order).packet(PacketType::MOD_ORDER).sendTo(peer, PacketChannel::CONNECT);
}

void ServerLuaParser::playerConnected(std::shared_ptr<ServerClient> client) {
    auto players = core.get<sol::table>("players");
    players.add(ServerLuaPlayer(*client));

    sol::object player = players[players.size()];

    safe_function(core["trigger"], "new_player", player);
    safe_function(core["trigger"], "player_join", player);
}

void ServerLuaParser::playerDisconnected(std::shared_ptr<ServerClient> client) {
    for (auto& pair : core.get<sol::table>("players")) {
        ServerLuaPlayer& p = pair.second.as<ServerLuaPlayer>();
        if (p.get_cid() == client->cid) {
            safe_function(core["trigger"], "player_disconnect", p);

            p.is_player = false;
            core.get<sol::table>("players")[pair.first] = sol::nil;
            break;
        }
    }
}

void ServerLuaParser::loadApi(ServerSubgame &defs, ServerWorld &world) {
    //Create Zepha Table
    core = lua.create_table();
    lua["zepha"] = core;
    core["__builtin"] = lua.create_table();

    // Types
    ServerApi::entity        (lua);
    ServerApi::server_player (lua);
    ServerApi::inventory     (lua);
    ClientApi::item_stack    (lua);

    Api::Usertype::Target::bind(Api::State::SERVER, lua, core);

    core["server"] = true;
    core["players"] = lua.create_table();

    // Modules
    modules.emplace_back(std::make_unique<Api::Module::Time>(Api::State::SERVER, lua, core));
    modules.emplace_back(std::make_unique<Api::Module::Block>(Api::State::SERVER, core, game, world));
    modules.emplace_back(std::make_unique<Api::Module::Entity>(Api::State::SERVER, core, game, world));
    modules.emplace_back(std::make_unique<Api::Module::Register>(Api::State::SERVER, core, game, world));

    Api::create_structure (lua, core);

    bindModules();

    // Create sandboxed runfile()
    lua["dofile"] = lua["loadfile"] = sol::nil;
    lua.set_function("runfile", &ServerLuaParser::runFileSandboxed, this);
}

void ServerLuaParser::registerDefs(ServerSubgame &defs) {
    RegisterBlocks::server(core, defs);
    RegisterItems ::server(core, defs);
    RegisterBiomes::server(core, defs);
}

sol::protected_function_result ServerLuaParser::errorCallback(sol::protected_function_result errPfr) const {
    sol::error err = errPfr;
    std::string errString = err.what();

    try {
        std::string::size_type slash = errString.find_first_of("/");
        if (slash != std::string::npos) throw "npos";

        std::string modString = errString.substr(0, slash);

        std::string::size_type lineNumStart = errString.find(':', slash);
        if (lineNumStart != std::string::npos) throw "lineNumStart";
        std::string::size_type lineNumEnd = errString.find(':', lineNumStart + 1);
        if (lineNumStart != std::string::npos) throw "lineNumEnd";

        std::string fileName = errString.substr(0, lineNumStart);
        int lineNum = std::stoi(errString.substr(lineNumStart + 1, lineNumEnd - lineNumStart - 1));

        for (auto& mod : handler.cGetMods()) {
            if (mod.config.name == modString) {
                for (auto& file : mod.files) {
                    if (file.path == fileName) {
                        std::cout << std::endl << ErrorFormatter::formatError(fileName, lineNum, errString, file.file) << std::endl;
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

sol::protected_function_result ServerLuaParser::runFileSandboxed(const std::string& file) {
    size_t modname_length = file.find('/');
    if (modname_length == std::string::npos) throw std::runtime_error("Error opening \"" + file + "\", specified file is invalid.");
    std::string modname = file.substr(0, modname_length);

    for (const LuaMod& mod : handler.cGetMods()) {
        if (modname != mod.config.name) continue;
        for (const LuaMod::File& f : mod.files) {
            if (f.path != file) continue;

            sol::environment env(lua, sol::create, lua.globals());
            env["_PATH"] = f.path.substr(0, f.path.find_last_of('/') + 1);
            env["_FILE"] = f.path;
            env["_MODNAME"] = mod.config.name;

            return lua.safe_script(f.file, env, std::bind(&ServerLuaParser::errorCallback, this, std::placeholders::_2), "@" + f.path, sol::load_mode::text);
        }
        throw std::runtime_error("Error opening \"" + file + "\", file not found.");
    }
    throw std::runtime_error("Error opening \"" + file + "\", mod not found.");
}