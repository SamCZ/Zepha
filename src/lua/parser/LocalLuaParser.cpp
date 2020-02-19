//
// Created by aurailus on 17/12/18.
//

#include "../ErrorFormatter.h"
#include "../register/RegisterBlocks.h"
#include "../register/RegisterItems.h"
#include "../register/RegisterBiomes.h"
#include "../register/RegisterKeybinds.h"

#include "LocalLuaParser.h"

// Types
#include "../api/type/LocalLuaPlayer.h"
#include "../api/type/cLocalLuaEntity.h"
#include "../api/type/cLuaLocalPlayer.h"
#include "../api/type/cLuaInventory.h"
#include "../api/type/cLuaItemStack.h"
#include "../api/type/cLocalLuaAnimationManager.h"

// Modules
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

// Functions
#include "../api/functions/trigger_event.h"
#include "../api/functions/update_entities.h"

void LocalLuaParser::init(ClientGame& defs, LocalWorld& world, Player& player) {
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);

    loadApi(defs, world, player);
    handler.executeMods(std::bind(&LocalLuaParser::runFileSandboxed, this, std::placeholders::_1));

    registerDefs(defs);
}

void LocalLuaParser::update(double delta, bool* keys) {
    LuaParser::update(delta);

    this->delta += delta;
    while (this->delta > double(UPDATE_STEP)) {
        manager.triggerKeybinds();
        core["__builtin"]["update_entities"](double(UPDATE_STEP));
        this->delta -= double(UPDATE_STEP);
    }

    manager.update(keys);
}

LocalModHandler& LocalLuaParser::getHandler() {
    return handler;
}

void LocalLuaParser::loadApi(ClientGame &defs, LocalWorld &world, Player& player) {
    //Create Zepha Table
    core = lua.create_table();
    lua["zepha"] = core;
    core["__builtin"] = lua.create_table();

    // Types
    ClientApi::entity(lua);
    ClientApi::animation_manager(lua);
    ClientApi::local_player(lua);
    ClientApi::inventory(lua);
    ClientApi::item_stack(lua);

    core["client"] = true;
    core["player"] = LocalLuaPlayer(player);

    // Modules
    Api::delay (core, delayed_functions);

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

    // Functions
    Api::update_entities(lua);

    // Create sandboxed runfile()
    lua["dofile"] = lua["loadfile"] = sol::nil;
    lua.set_function("runfile", &LocalLuaParser::runFileSandboxed, this);
}

void LocalLuaParser::registerDefs(ClientGame &defs) {
    RegisterBlocks  ::client(core, defs);
    RegisterItems   ::client(core, defs);
    RegisterBiomes  ::client(core, defs);
    RegisterKeybinds::client(core, manager);
}

sol::protected_function_result LocalLuaParser::errorCallback(lua_State*, sol::protected_function_result errPfr) {
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

    for (const auto& mod : handler.cGetMods()) {
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

sol::protected_function_result LocalLuaParser::runFileSandboxed(std::string file) {
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

                    return lua.safe_script(f.file, env, std::bind(&LocalLuaParser::errorCallback, this,
                            std::placeholders::_1, std::placeholders::_2), "@" + f.path, sol::load_mode::text);
                }
            }

            std::cout << Log::err << "Error opening \"" + file + "\", not found." << Log::endl;
            break;
        }
    }
}
