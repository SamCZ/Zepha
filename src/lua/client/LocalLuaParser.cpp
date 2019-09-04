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
    //Create Zeus Table
    zeus = lua.create_table();
    lua["zeus"] = zeus;

    //Load Types
    ClientApi::entity(lua, zeus, defs, world);

    //Load Modules
    ClientApi::dump(lua);
    ClientApi::printe(lua);

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
                    lua.safe_script(f.file);
                    return 0;
                }
            }
            break;
        }
    }

    std::cout << Log::err << "Error opening \"" + file + "\", not found." << Log::endl;
}