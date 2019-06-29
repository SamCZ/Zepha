//
// Created by aurailus on 11/06/19.
//

#include "ServerLuaParser.h"
#include "../../def/ServerDefs.h"

#include "ModuleServerRegisterBlock.h"
#include "ModuleServerUtils.h"
#include "ModuleServerRegisterFunctions.h"
#include "ModuleServerGetSetBlock.h"
#include "ModuleServerDelay.h"

ServerLuaParser::ServerLuaParser(std::string mod_root) : LuaParser(std::move(mod_root)) {}

void ServerLuaParser::init(ServerDefs& defs, ServerWorld& world) {
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);

    zeus = lua.create_table();
    lua["zeus"] = zeus;

    lua.set_function("dofile", &ServerLuaParser::DoFileSandboxed, this);

    ModuleServerUtils(lua, zeus, defs);
    ModuleServerRegisterFunctions(lua, zeus, defs);
    ModuleServerRegisterBlock(lua, zeus, defs);
    ModuleServerGetSetBlock(lua, zeus, defs, world);
    ModuleServerDelay(lua, zeus, defs, delayed_functions);

    //LOAD MODS

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

void ServerLuaParser::update() {
    auto it = delayed_functions.begin();
    while (it != delayed_functions.end()) {
        DelayedFunction& f = *it;
        f.timeout -= 0.048f;
        if (f.timeout <= 0) {
            if (f.function(sol::as_args(f.args))) {
                f.timeout = f.initial_timeout;
            } else {
                it = delayed_functions.erase(it);
                continue;
            }
        }
        it++;
    }
}
