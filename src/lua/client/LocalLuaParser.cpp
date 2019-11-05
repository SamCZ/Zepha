//
// Created by aurailus on 17/12/18.
//

#include "LocalLuaParser.h"
#include "LocalRegisterBlocks.h"
#include "LocalRegisterItems.h"
#include "LocalRegisterKeybinds.h"

#include "../../def/LocalDefs.h"
#include "../api/type/LuaLocalPlayer.h"
#include "../../game/scene/world/Player.h"

#include "../api/type/cLuaEntity.h"
#include "../api/type/cLuaLocalPlayer.h"

#include "../api/modules/cDump.h"
#include "../api/modules/cPrintE.h"

#include "../api/modules/cIsServer.h"
#include "../api/modules/cDelay.h"

#include "../api/modules/cRegisterBlock.h"
#include "../api/modules/cRegisterBlockmodel.h"
#include "../api/modules/cRegisterItem.h"
#include "../api/modules/cRegisterEntity.h"

#include "../api/modules/cSetBlock.h"
#include "../api/modules/cGetBlock.h"
#include "../api/modules/cRemoveBlock.h"

#include "../api/modules/cAddEntity.h"
#include "../api/modules/cRemoveEntity.h"

#include "../api/modules/cRegisterKeybind.h"

#include "../api/functions/cUpdateEntities.h"

void LocalLuaParser::init(LocalDefs& defs, LocalWorld& world, Player& player) {
    //Load Base Libraries
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);

    //Define Panic Callback
//    lua_atpanic(lua, sol::c_call<decltype(&LuaParser::override_panic), &LuaParser::override_panic>);

    //Load Modules
    loadModules(defs, world, player);

    //Load Mods
    loadMods();

    //Register Blocks
    registerDefinitions(defs);
}

void LocalLuaParser::loadModules(LocalDefs &defs, LocalWorld &world, Player& player) {
    //Create Zepha Table
    core = lua.create_table();
    lua["zepha"] = core;
    core["__builtin"] = lua.create_table();

    //Load Types
    ClientApi::entity(lua, world);
    ClientApi::local_player(lua, world);

    core["player"] = LuaLocalPlayer(player);

    //Load Modules
    ClientApi::dump(lua);
    ClientApi::printe(lua);

    ClientApi::is_server(core);
    ClientApi::delay(core, delayed_functions);

    ClientApi::register_block(lua, core);
    ClientApi::register_blockmodel(lua, core);
    ClientApi::register_item(lua, core);
    ClientApi::register_entity(lua, core);

    ClientApi::get_block(core, defs, world);
    ClientApi::set_block(core, defs, world);
    ClientApi::remove_block(core, defs, world);

    ClientApi::add_entity(lua, core, defs, world);
    ClientApi::remove_entity(lua, core, defs, world);

    ClientApi::register_keybind(lua, core);

    ClientApi::update_entities(lua);

    //Sandbox the dofile function
    lua["dofile"] = sol::nil;
    lua.set_function("runfile", &LocalLuaParser::DoFileSandboxed, this);
}

void LocalLuaParser::loadMods() {
    //Load "base" if it exists.
    for (const std::string& modName : modsOrder) {
        if (modName == "base") {
            DoFileSandboxed(modName + "/main");
            break;
        }
    }

    for (const std::string& modName : modsOrder) {
        if (modName != "base") {
            DoFileSandboxed(modName + "/main");
        }
    }
}

void LocalLuaParser::registerDefinitions(LocalDefs &defs) {
    LocalRegisterBlocks(core, defs);
    LocalRegisterItems(core, defs);
    LocalRegisterKeybinds(core, defs, manager);
}

void LocalLuaParser::update(double delta, bool* keys) {
    LuaParser::update(delta);
    this->delta += delta;
    while (this->delta > 0.05f) {
        core["__builtin"]["update_entities"](this->delta);
        manager.triggerKeybinds();
        this->delta -= 0.05f;
    }
    manager.update(keys);
}

sol::protected_function_result LocalLuaParser::DoFileSandboxed(std::string file) {
    size_t modname_length = file.find('/');
    std::string modname = file.substr(0, modname_length);

    for (LuaMod& mod : mods) {
        if (strncmp(mod.config.name.c_str(), modname.c_str(), modname_length) == 0) {
            for (LuaModFile& f : mod.files) {
                if (f.path == file) {

                    sol::environment env(lua, sol::create, lua.globals());
                    env["_PATH"] = f.path.substr(0, f.path.find_last_of('/') + 1);
                    env["_FILE"] = f.path;
                    env["_MODNAME"] = mod.config.name;

                    auto pfr = lua.safe_script(f.file, env, [&](lua_State*, sol::protected_function_result errPfr) {
                        sol::error err = errPfr;
                        std::cout << Log::err << file << " returned an error: " << err.what() << Log::endl;
                        return errPfr;
                    }, "@" + f.path, sol::load_mode::text);

                    return pfr;
                }
            }

            std::cout << Log::err << "Error opening \"" + file + "\", not found." << Log::endl;
            break;
        }
    }
}