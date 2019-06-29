//
// Created by aurailus on 17/12/18.
//

#include "LocalLuaParser.h"
#include "LocalRegisterBlocks.h"
#include "../../def/LocalDefs.h"

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

void LocalLuaParser::init(LocalDefs& defs, LocalWorld& world) {
    //Load Base Libraries
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);

    //Define Panic Callback
    lua_atpanic(lua, sol::c_call<decltype(&LuaParser::override_panic), &LuaParser::override_panic>);

    //Load Modules
    loadModules(defs, world);

    //Load Mods
    loadMods();

    //Register Blocks
    registerBlocks(defs);
}

void LocalLuaParser::loadModules(LocalDefs &defs, LocalWorld &world) {
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
}

void LocalLuaParser::loadMods() {
    //Sandbox the dofile function
    lua.set_function("dofile", &LocalLuaParser::DoFileSandboxed, this);

    cf_dir_t mods_dir;
    cf_dir_open(&mods_dir, root.c_str());

    //Iterate through all mod folders
    while (mods_dir.has_next) {
        cf_file_t mod_folder_file;
        cf_read_file(&mods_dir, &mod_folder_file);

        if (mod_folder_file.is_dir && strncmp(mod_folder_file.name, ".", 2) != 0 && strncmp(mod_folder_file.name, "..", 3) != 0) {
            cf_dir_t mod_dir;
            cf_dir_open(&mod_dir, mod_folder_file.path);

            //Find main.lua
            while (mod_dir.has_next) {
                cf_file_t mod_file;
                cf_read_file(&mod_dir, &mod_file);

                if (strncmp(mod_file.name, "main.lua", 9) == 0) {
                    //Run main.lua
                    DoFileSandboxed(mod_file.path);
                    break;
                }

                cf_dir_next(&mod_dir);
            }
        }
        cf_dir_next(&mods_dir);
    }
    cf_dir_close(&mods_dir);
}

void LocalLuaParser::registerBlocks(LocalDefs& defs) {
    LocalRegisterBlocks(zeus, defs);
}
