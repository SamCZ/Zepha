//
// Created by aurailus on 17/12/18.
//

#include "LocalLuaParser.h"

#include "ErrorFormatter.h"
#include "../game/ClientState.h"
#include "../game/graph/Renderer.h"
#include "register/RegisterItems.h"
#include "register/RegisterBlocks.h"
#include "register/RegisterBiomes.h"
#include "register/RegisterKeybinds.h"

// Usertypes
#include "usertype/Target.h"
#include "usertype/Player.h"
#include "usertype/Entity.h"
#include "usertype/Inventory.h"
#include "usertype/Dimension.h"
#include "usertype/ItemStack.h"
#include "usertype/InventoryList.h"
#include "usertype/AnimationManager.h"

#include "usertype/LuaGuiElement.h"

// Modules
#include "modules/Time.h"
#include "modules/Register.h"
#include "modules/Dimension.h"

#include "modules/create_structure.h"

LocalLuaParser::LocalLuaParser(LocalSubgame& game): LuaParser(game), keybinds(this) {}

void LocalLuaParser::init(WorldPtr world, PlayerPtr player, ClientState& state) {
    lua.open_libraries(sol::lib::base, sol::lib::string, sol::lib::math, sol::lib::table);

    loadApi(world, player);
    handler.executeMods(Util::bind_this(this, &LocalLuaParser::runFileSandboxed));
    state.renderer.window.input.setCallback(Util::bind_this(&keybinds, &LuaKeybindHandler::keybindHandler));

    registerDefs();
}

void LocalLuaParser::update(double delta) {
    this->delta += delta;
    while (this->delta > static_cast<double>(UPDATE_STEP)) {
        safe_function(core["__builtin"]["update_entities"], static_cast<double>(UPDATE_STEP));
        safe_function(core["__builtin"]["update_delayed_functions"]);
        this->delta -= static_cast<double>(UPDATE_STEP);
    }
}

LocalModHandler& LocalLuaParser::getHandler() {
    return handler;
}

void LocalLuaParser::loadApi(WorldPtr world, PlayerPtr player) {
    //Create Zepha Table
    core = lua.create_table();
    lua["zepha"] = core;
    core["__builtin"] = lua.create_table();

    // Types
    ClientApi::gui_element       (lua);

    Api::Usertype::Target::bind(Api::State::CLIENT, lua, core);
    Api::Usertype::Entity::bind(Api::State::CLIENT, lua, core);
    Api::Usertype::Inventory::bind(Api::State::CLIENT, lua, core);
    Api::Usertype::Dimension::bind(Api::State::CLIENT, lua, core);
    Api::Usertype::ItemStack::bind(Api::State::CLIENT, lua, core);
    Api::Usertype::LocalPlayer::bind(Api::State::CLIENT, lua, core);
    Api::Usertype::InventoryList::bind(Api::State::CLIENT, lua, core);
    Api::Usertype::AnimationManager::bind(Api::State::CLIENT, lua, core);

    core["client"] = true;
    core["player"] = Api::Usertype::LocalPlayer(player);

    // Modules
    modules.emplace_back(std::make_unique<Api::Module::Time>(Api::State::CLIENT, lua, core));
    modules.emplace_back(std::make_unique<Api::Module::Register>(Api::State::CLIENT, core, game, **world));
    modules.emplace_back(std::make_unique<Api::Module::Dimension>(Api::State::CLIENT, core, game, **world));

    bindModules();

    Api::create_structure (lua, core);

    // Create sandboxed runfile()
    lua["dofile"] = lua["loadfile"] = sol::nil;
    lua.set_function("runfile", &LocalLuaParser::runFileSandboxed, this);
}

void LocalLuaParser::registerDefs() {
    auto& local = static_cast<LocalSubgame&>(game);
    RegisterBlocks  ::client(core, local);
    RegisterItems   ::client(core, local);
    RegisterBiomes  ::client(core, local);
    RegisterKeybinds::client(core, keybinds);
}

sol::protected_function_result LocalLuaParser::errorCallback(sol::protected_function_result r) const {
    sol::error err = r;
    std::string errString = err.what();

    try {
        std::string::size_type slash = errString.find('/');
        if (slash != std::string::npos) throw "npos";

        std::string modString = errString.substr(0, slash);

        std::string::size_type lineNumStart = errString.find(':', slash);
        if (lineNumStart != std::string::npos) throw "lineNumStart";
        std::string::size_type lineNumEnd = errString.find(':', lineNumStart + 1);
        if (lineNumStart != std::string::npos) throw "lineNumEnd";

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
    }
    catch (...) {
        std::cout << Log::err << "Zepha has encountered an error, and ErrorFormatter failed to format it:"
            << std::endl << std::endl << errString << Log::endl;
    }

    throw std::runtime_error("Exiting.");
}

sol::protected_function_result LocalLuaParser::runFileSandboxed(const std::string& file) {
    size_t modname_length = file.find('/');
    if (modname_length == std::string::npos) throw std::runtime_error("Error opening \"" + file + "\", specified file is invalid.");
    std::string modname = file.substr(0, modname_length);

    for (const LuaMod& mod : handler.cGetMods()) {
        if (modname != mod.config.name) continue;
        for (const LuaMod::File& f : mod.files) {
            if (f.path != file) continue;

            sol::environment env(lua, sol::create, lua.globals());
            env["_PATH"] = f.path.substr(0, f.path.find_last_of('/') + 1);
            env["_FILE"] = f.path;
            env["_MODNAME"] = mod.config.name;

            return lua.safe_script(f.file, env, std::bind(&LocalLuaParser::errorCallback, this, std::placeholders::_2), "@" + f.path, sol::load_mode::text);
        }
        throw std::runtime_error("Error opening \"" + file + "\", file not found.");
    }
    throw std::runtime_error("Error opening \"" + file + "\", mod not found.");
}