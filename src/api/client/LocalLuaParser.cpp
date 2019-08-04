//
// Created by aurailus on 17/12/18.
//

#include "LocalLuaParser.h"
#include "LocalRegisterBlocks.h"
#include "../../def/LocalDefs.h"
#include "../../game/entity/hud/GameGui.h"

#include "modules/cDump.h"
#include "modules/cPrintE.h"

#include "modules/cGetPath.h"
#include "modules/cIsServer.h"
#include "modules/cDelay.h"

#include "modules/cRegisterBlock.h"
#include "modules/cRegisterBlockmodel.h"

#include "modules/cSetBlock.h"
#include "modules/cGetBlock.h"
#include "modules/cRemoveBlock.h"

#include "modules/cShowMenu.h"

void LocalLuaParser::init(LocalDefs& defs, LocalWorld& world, GameGui& gui) {
    //Load Base Libraries
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);

    //Define Panic Callback
    lua_atpanic(lua, sol::c_call<decltype(&LuaParser::override_panic), &LuaParser::override_panic>);

    //Load Modules
    loadModules(defs, world, gui);

    //Load Mods
    loadMods();

    //Register Blocks
    registerBlocks(defs);
}

void LocalLuaParser::loadModules(LocalDefs &defs, LocalWorld &world, GameGui& gui) {
    //Create Zeus Table
    zeus = lua.create_table();
    lua["zeus"] = zeus;

    //Load Modules
    ClientApi::dump(lua);
    ClientApi::printe(lua);

    ClientApi::get_path(zeus, root);
    ClientApi::is_server(zeus);
    ClientApi::delay(zeus, delayed_functions);

    ClientApi::register_block(lua, zeus);
    ClientApi::register_blockmodel(lua, zeus);

    ClientApi::get_block(zeus, defs, world);
    ClientApi::set_block(zeus, defs, world);
    ClientApi::remove_block(zeus, defs, world);

    ClientApi::show_menu(zeus, defs, gui);

    //Sandbox the dofile function
    lua.set_function("dofile", &LocalLuaParser::DoFileSandboxed, this);
}

void LocalLuaParser::loadMods() {
    for (const std::string& modName : modsOrder) {
        DoFileSandboxed(modName + "/main");
    }
}

void LocalLuaParser::registerBlocks(LocalDefs& defs) {
    LocalRegisterBlocks(zeus, defs);
}

int LocalLuaParser::DoFileSandboxed(std::string file) {
    size_t modname_length = file.find('/');
    std::string modname = file.substr(0, modname_length);

    for (LuaMod& mod : mods) {
        if (strncmp(mod.config.name.c_str(), modname.c_str(), modname_length) == 0) {
            for (LuaModFile& f : mod.files) {
                if (f.path == file) {
                    lua.script(f.file);
                    return 0;
                }
            }
            break;
        }
    }

    std::cout << Log::err << "Error opening \"" + file + "\", not found." << Log::endl;
}