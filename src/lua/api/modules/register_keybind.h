//
// Created by aurailus on 12/10/19.
//

#pragma once

#include <sol2/sol.hpp>

namespace Api {
    static void register_keybind(sol::state& lua, sol::table& core) {
        core["registered_keybinds"] = lua.create_table();
        core["keys"] = lua.create_table();
        core["keycodes"] = lua.create_table();

        for (unsigned short i = 0; i < 350; i++) {
            auto key = Util::getKeyStr(i);
            if (!key.empty()) {
                core["keys"][key] = i;
                core["keycodes"][i] = key;
            }
        }

        core.set_function("register_keybind", [&](sol::this_environment env, sol::optional<std::string> identifier, sol::optional<sol::table> data) {
            if (!identifier || !identifier->length()) throw "expected a string as the first argument.";
            if (!data) throw "expected a table as the second argument.";

            auto modname = static_cast<sol::environment>(env).get<std::string>("_MODNAME");

            if (identifier->compare(0, modname.length() + 1, modname + ":")) throw
                        "identifier '" + *identifier + "' must begin with the calling mod's prefix, '" + modname + "'.";

            core["registered_keybinds"][identifier] = data;
        });
    }
}