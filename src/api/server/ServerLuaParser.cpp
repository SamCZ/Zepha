//
// Created by aurailus on 11/06/19.
//

#include "ServerLuaParser.h"
#include "ServerRegisterBlocks.h"
#include "../../def/ServerDefs.h"

#include "modules/sDump.h"
#include "modules/sPrintE.h"

#include "modules/sGetPath.h"
#include "modules/sIsServer.h"
#include "modules/sDelay.h"

#include "modules/sRegisterBlock.h"
#include "modules/sRegisterBlockmodel.h"

#include "modules/sSetBlock.h"
#include "modules/sGetBlock.h"
#include "modules/sRemoveBlock.h"

void ServerLuaParser::init(ServerDefs& defs, ServerWorld& world) {
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

void ServerLuaParser::loadModules(ServerDefs &defs, ServerWorld &world) {
    //Create Zeus Table
    zeus = lua.create_table();
    lua["zeus"] = zeus;

    //Load Modules
    ServerApi::dump(lua);
    ServerApi::printe(lua);

    ServerApi::get_path(zeus, root);
    ServerApi::is_server(zeus);
    ServerApi::delay(zeus, delayed_functions);

    ServerApi::register_block(lua, zeus);
    ServerApi::register_blockmodel(lua, zeus);

    ServerApi::get_block(zeus, defs, world);
    ServerApi::set_block(zeus, defs, world);
    ServerApi::remove_block(zeus, defs, world);
}

void ServerLuaParser::loadMods() {
    //Sandbox the dofile function
    lua.set_function("dofile", &ServerLuaParser::DoFileSandboxed, this);

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

void ServerLuaParser::registerBlocks(ServerDefs& defs) {
    ServerRegisterBlocks(zeus, defs);
}
