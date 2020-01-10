//
// Created by aurailus on 17/12/18.
//

#include "LocalLuaParser.h"
#include "LocalRegisterBlocks.h"
#include "LocalRegisterItems.h"
#include "LocalRegisterKeybinds.h"
#include "LocalRegisterBiomes.h"

#include "../../def/LocalDefs.h"
#include "../api/type/LocalLuaPlayer.h"

#include "../api/type/cLocalLuaEntity.h"
#include "../api/type/cLuaLocalPlayer.h"
#include "../api/type/cLuaInventory.h"
#include "../api/type/cLuaItemStack.h"

#include "../api/modules/delay.h"

#include "../api/modules/register_block.h"
#include "../api/modules/register_blockmodel.h"
#include "../api/modules/register_biome.h"
#include "../api/modules/register_item.h"
#include "../api/modules/register_entity.h"

#include "../api/modules/set_block.h"
#include "../api/modules/get_block.h"
#include "../api/modules/remove_block.h"

#include "../api/modules/add_entity.h"
#include "../api/modules/remove_entity.h"

#include "../api/modules/register_keybind.h"

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
    ClientApi::inventory(lua);
    ClientApi::item_stack(lua);

    core["client"] = true;
    core["player"] = LocalLuaPlayer(player);

    //Load Modules
    Api::delay(core, delayed_functions);

    Api::register_block      (lua, core);
    Api::register_blockmodel (lua, core);
    Api::register_biome      (lua, core);
    Api::register_item       (lua, core);
    Api::register_entity     (lua, core);
    Api::register_keybind    (lua, core);

    Api::get_block    (core, defs.defs, world);
    Api::set_block    (core, defs.defs, world);
    Api::remove_block (core, defs.defs, world);

    Api::add_entity_c    (lua, core, defs, world);
    Api::remove_entity_c (lua, core, defs, world);

    ClientApi::update_entities(lua);

    //Sandbox the dofile function
    lua["dofile"] = lua["loadfile"] = sol::nil;
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
    LocalRegisterBiomes(core, defs);
}

void LocalLuaParser::update(double delta, bool* keys) {
    LuaParser::update(delta);

    this->delta += delta;
    while (this->delta > double(UPDATE_STEP)) {
        core["__builtin"]["update_entities"](double(UPDATE_STEP));
        manager.triggerKeybinds();
        this->delta -= double(UPDATE_STEP);
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