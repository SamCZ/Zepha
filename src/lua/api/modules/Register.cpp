//
// Created by aurailus on 2020-07-24.
//

#include "Register.h"

#include "../../Lua.h"

void Api::Module::Register::bind() {

    // Basic

    createRegisterFn("item");
    createRegisterFn("block");
    createRegisterFn("biome");
    createRegisterFn("blockmodel");

    // Keybinds

    core["keys"] = lua.create_table();
    core["keycodes"] = lua.create_table();

    for (unsigned short i = 0; i < 350; i++) {
        auto key = Util::getKeyStr(i);
        if (!key.empty()) {
            core["keys"][key] = i;
            core["keycodes"][i] = key;
        }
    }

    createRegisterFn("keybind");

    // Entities

    core["registered_entities"] = lua.create_table();
    core.set_function("register_entity", [=](sol::this_environment env, std::string identifier, sol::table data) {
        data["__index"] = data;
        registerFn("registered_entities", static_cast<sol::environment>(env), identifier, data);
    });
}

void Api::Module::Register::createRegisterFn(const std::string& name, const std::string& table) {
    std::string tableName = "registered_" +  (table.empty() ? name + "s" : table);
    core[tableName] = lua.create_table();
    core.set_function("register_" + name, [=](sol::this_environment env, std::string identifier, sol::table data)
        { registerFn(tableName, static_cast<sol::environment>(env), identifier, data); });
}

void Api::Module::Register::registerFn(const std::string& table, sol::environment env, const std::string& identifier, const sol::table& data) {
    auto modName = env.get<std::string>("_MODNAME");

    if (identifier[0] != ':' && strncmp(identifier.data(), modName.data(), modName.length()))
        throw std::runtime_error(identifier + " does not match calling mod name.");

    std::string iden = (identifier[0] == ':' ? modName + identifier : identifier);
    unsigned int splitters = std::count_if(iden.begin(), iden.end(), [](char c) { return c == ':'; });

    if (splitters > 2) throw std::runtime_error("Too many splitters in identifier " + iden + ".");

    core[table][iden] = data;
}
