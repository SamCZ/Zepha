//
// Created by aurailus on 17/12/18.
//

#include "LocalLuaParser.h"
#include "LocalRegisterBlocks.h"
#include "../../def/LocalDefs.h"
#include "../../game/hud/GameGui.h"

#include "../api/type/cEntity.h"

#include "../api/modules/cDump.h"
#include "../api/modules/cPrintE.h"

#include "../api/modules/cIsServer.h"
#include "../api/modules/cDelay.h"

#include "../api/modules/cRegisterBlock.h"
#include "../api/modules/cRegisterBlockmodel.h"

#include "../api/modules/cSetBlock.h"
#include "../api/modules/cGetBlock.h"
#include "../api/modules/cRemoveBlock.h"

#include "../api/modules/cShowMenu.h"

void LocalLuaParser::init(LocalDefs& defs, LocalWorld& world, GameGui& gui) {
    //Load Base Libraries
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);

    //Define Panic Callback
//    lua_atpanic(lua, sol::c_call<decltype(&LuaParser::override_panic), &LuaParser::override_panic>);

    //Load Modules
    loadModules(defs, world, gui);

    //Load Mods
    loadMods();

    //Register Blocks
    registerBlocks(defs);
}

void LocalLuaParser::loadModules(LocalDefs &defs, LocalWorld &world, GameGui& gui) {
    //Create Zepha Table
    core = lua.create_table();
    lua["zepha"] = core;

    //Load Types
    ClientApi::entity(lua, core, defs, world);

    //Load Modules
    ClientApi::dump(lua);
    ClientApi::printe(lua);

    ClientApi::is_server(core);
    ClientApi::delay(core, delayed_functions);

    ClientApi::register_block(lua, core);
    ClientApi::register_blockmodel(lua, core);

    ClientApi::get_block(core, defs, world);
    ClientApi::set_block(core, defs, world);
    ClientApi::remove_block(core, defs, world);

    ClientApi::show_menu(core, defs, gui);

    //Sandbox the dofile function
    lua["dofile"] = sol::nil;
    lua.set_function("runfile", &LocalLuaParser::DoFileSandboxed, this);
}

void LocalLuaParser::loadMods() {
    for (const std::string& modName : modsOrder) {
        DoFileSandboxed(modName + "/main");
    }
}

void LocalLuaParser::registerBlocks(LocalDefs& defs) {
    LocalRegisterBlocks(core, defs);
}

sol::protected_function_result LocalLuaParser::DoFileSandboxed(std::string file) {
    size_t modname_length = file.find('/');
    std::string modname = file.substr(0, modname_length);

    for (LuaMod& mod : mods) {
        if (strncmp(mod.config.name.c_str(), modname.c_str(), modname_length) == 0) {
            for (LuaModFile& f : mod.files) {
                if (f.path == file) {
                    auto pfr = lua.safe_script(f.file, [&](lua_State*, sol::protected_function_result errPfr) {
                        sol::error err = errPfr;
                        std::cout << Log::err << file << " returned an error: " << err.what() << Log::endl;
                        return errPfr;
                    });
                    return pfr;
                }
            }
            // TODO: No file found by that name - throw error?
            std::cout << Log::err << "Error opening \"" + file + "\", not found." << Log::endl;
            break;
        }
    }
}