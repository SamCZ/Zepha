//
// Created by aurailus on 12/10/19.
//

#pragma once

#include <sol2/sol.hpp>

namespace ServerApi {
    void register_keybind(sol::state& lua, sol::table& core) {
        core["registered_keybinds"] = lua.create_table();

        core.set_function("register_keybind", [&](std::string identifier, sol::table data) {
            if (identifier.length() == 0) throw "Missing Identifier";
            core["registered_keybinds"][identifier] = data;
        });
    }
}