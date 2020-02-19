//
// Created by aurailus on 11/06/19.
//

#include <gzip/compress.hpp>
#include <enet/enet.h>

#include "../ErrorFormatter.h"
#include "../register/RegisterBlocks.h"
#include "../register/RegisterItems.h"
#include "../register/RegisterBiomes.h"

#include "ServerLuaParser.h"

// Types
#include "../api/type/sServerLuaPlayer.h"
#include "../api/type/sServerLuaEntity.h"
#include "../api/type/cLuaInventory.h"
#include "../api/type/cLuaItemStack.h"

// Modules
#include "../api/modules/delay.h"
#include "../api/modules/register_block.h"
#include "../api/modules/register_blockmodel.h"
#include "../api/modules/register_biome.h"
#include "../api/modules/register_item.h"
#include "../api/modules/register_entity.h"
#include "../api/modules/register_keybind.h"
#include "../api/modules/register_on.h"
#include "../api/modules/set_block.h"
#include "../api/modules/get_block.h"
#include "../api/modules/remove_block.h"
#include "../api/modules/add_entity.h"
#include "../api/modules/remove_entity.h"

// Functions
#include "../api/functions/trigger_event.h"
#include "../api/functions/update_entities.h"


void ServerLuaParser::init(ServerGame& defs, ServerWorld& world, std::string path) {
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);

    loadApi(defs, world);
    handler.loadMods(defs, path + "mods");
    handler.executeMods(std::bind(&ServerLuaParser::runFileSandboxed, this, std::placeholders::_1));

    registerDefs(defs);

    std::cout << Log::info << "Loaded " << handler.cGetMods().size() << " mods: [ ";
    for (unsigned int i = 0; i < handler.cGetMods().size(); i++)
        std::cout << handler.cGetMods()[i].config.name << (i < handler.cGetMods().size() - 1 ? ", " : " ]\n");
}

void ServerLuaParser::update(double delta) {
    LuaParser::update(delta);

    this->delta += delta;
    while (this->delta > double(UPDATE_STEP)) {
        core["__builtin"]["update_entities"](double(UPDATE_STEP));
        this->delta -= double(UPDATE_STEP);
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
    core["__builtin"]["trigger_event"]("player_join", players[players.size()]);
    core["__builtin"]["trigger_event"]("player_connect", players[players.size()]);
}

void ServerLuaParser::playerDisconnected(std::shared_ptr<ServerClient> client) {
    for (auto& pair : core.get<sol::table>("players")) {
        ServerLuaPlayer& p = pair.second.as<ServerLuaPlayer>();
        if (p.cid == client->cid) {
            core["__builtin"]["trigger_event"]("player_disconnect", p);

            p.is_player = false;
            core.get<sol::table>("players")[pair.first] = sol::nil;
            break;
        }
    }
}

void ServerLuaParser::loadApi(ServerGame &defs, ServerWorld &world) {
    //Create Zepha Table
    core = lua.create_table();
    lua["zepha"] = core;
    core["__builtin"] = lua.create_table();

    // Types
    ServerApi::entity(lua);
    ServerApi::server_player(lua);
    ClientApi::inventory(lua);
    ClientApi::item_stack(lua);

    core["server"] = true;
    core["players"] = lua.create_table();

    // Modules
    Api::delay (core, delayed_functions);

    Api::register_block      (lua, core);
    Api::register_blockmodel (lua, core);
    Api::register_biome      (lua, core);
    Api::register_item       (lua, core);
    Api::register_entity     (lua, core);
    Api::register_keybind    (lua, core);
    Api::register_on_s       (lua, core, *this);

    Api::get_block    (core, defs.defs, world);
    Api::set_block    (core, defs.defs, world);
    Api::remove_block (core, defs.defs, world);

    Api::add_entity_s    (lua, core, defs, world);
    Api::remove_entity_s (lua, core, defs, world);

    // Functions
    Api::trigger_event(lua);
    Api::update_entities(lua);

    // Create sandboxed runfile()
    lua["dofile"] = lua["loadfile"] = sol::nil;
    lua.set_function("runfile", &ServerLuaParser::runFileSandboxed, this);
}

void ServerLuaParser::registerDefs(ServerGame &defs) {
    RegisterBlocks::server(core, defs);
    RegisterItems ::server(core, defs);
    RegisterBiomes::server(core, defs);
}

sol::protected_function_result ServerLuaParser::errorCallback(lua_State*, sol::protected_function_result errPfr) {
    sol::error err = errPfr;
    std::string errString = err.what();

    std::string::size_type slash = errString.find('/');
    assert(slash != std::string::npos);

    std::string modString = errString.substr(0, slash);

    std::string::size_type lineNumStart = errString.find(':', slash);
    assert(lineNumStart != std::string::npos);
    std::string::size_type lineNumEnd = errString.find(':', lineNumStart + 1);
    assert(lineNumEnd != std::string::npos);

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

    exit(1);
    return errPfr;
}

sol::protected_function_result ServerLuaParser::runFileSandboxed(const std::string& file) {
    size_t modname_length = file.find('/');
    std::string modname = file.substr(0, modname_length);

    for (const LuaMod& mod : handler.cGetMods()) {
        if (strncmp(mod.config.name.c_str(), modname.c_str(), modname_length) == 0) {
            for (const LuaModFile& f : mod.files) {
                if (f.path == file) {

                    sol::environment env(lua, sol::create, lua.globals());
                    env["_PATH"] = f.path.substr(0, f.path.find_last_of('/') + 1);
                    env["_FILE"] = f.path;
                    env["_MODNAME"] = mod.config.name;

                    return lua.safe_script(f.file, env, std::bind(&ServerLuaParser::errorCallback, this,
                            std::placeholders::_1, std::placeholders::_2), "@" + f.path, sol::load_mode::text);
                }
            }

            std::cout << Log::err << "Error opening \"" + file + "\", not found." << Log::endl;
            break;
        }
    }
}