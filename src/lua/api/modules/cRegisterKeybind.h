//
// Created by aurailus on 11/10/19.
//

#pragma once

#include <sol2/sol.hpp>

namespace ClientApi {
    void register_keybind(sol::state& lua, sol::table& core) {
        core["registered_keybinds"] = lua.create_table();
        core["keys"] = lua.create_table();

//        for (ushort i = 0; i < 350; i++) {
//            switch (i) {
//                case
//            }
//        }

        core.set_function("register_keybind", [&](std::string identifier, sol::table data) {
            if (identifier.length() == 0) throw "Missing Identifier";
            core["registered_keybinds"][identifier] = data;
        });
    }
}