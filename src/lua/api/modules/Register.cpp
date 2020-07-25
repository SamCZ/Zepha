//
// Created by aurailus on 2020-07-24.
//

#include "Register.h"

#include "../../Lua.h"

void Api::Module::Register::bind() {
    createRegisterFn("block", "blocks");
    createRegisterFn("biome", "biomes");
    createRegisterFn("entity", "entities");
    createRegisterFn("keybind", "keybinds");
    createRegisterFn("blockmodel", "blockmodels");
}

void Api::Module::Register::createRegisterFn(const std::string &name, const std::string &table) {
    core[table] = lua.create_table();
    core.set_function("register_" + name, [=](sol::this_environment env, std::string identifier, sol::table data)
        { registerFn(table, static_cast<sol::environment>(env), identifier, data); });
}

void Api::Module::Register::registerFn(const std::string& table, sol::environment env, const std::string& identifier, const sol::table& data) {
    std::cout << env.get<std::string>("_MODNAME");

    core[table][identifier] = data;
}
