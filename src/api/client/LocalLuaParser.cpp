//
// Created by aurailus on 17/12/18.
//

#include "LocalLuaParser.h"
#include "../../def/LocalDefs.h"

#include "ModuleClientRegisterBlock.h"
#include "ModuleClientUtils.h"
#include "ModuleClientRegisterBlockModel.h"
#include "ModuleClientGetSetBlock.h"
#include "ModuleClientDelay.h"

LocalLuaParser::LocalLuaParser(std::string mod_root) : LuaParser(std::move(mod_root)) {}

void LocalLuaParser::init(LocalDefs& defs, LocalWorld& world) {
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math);

    zeus = lua.create_table();
    lua["zeus"] = zeus;

    lua.set_function("dofile", &LocalLuaParser::DoFileSandboxed, this);

    ModuleClientUtils(lua, zeus, defs);
    ModuleClientRegisterBlockModel(lua, zeus, defs);
    ModuleClientRegisterBlock(lua, zeus, defs);
    ModuleClientGetSetBlock(lua, zeus, defs, world);
    ModuleClientDelay(lua, zeus, defs, delayed_functions);

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

void LocalLuaParser::update() {
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
