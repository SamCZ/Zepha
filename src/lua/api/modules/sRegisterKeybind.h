//
// Created by aurailus on 12/10/19.
//

#pragma once

#include <sol2/sol.hpp>

namespace ServerApi {
    void register_keybind(sol::state& lua, sol::table& core) {
        core["registered_keybinds"] = lua.create_table();
        core["keys"] = lua.create_table();
        core["keycodes"] = lua.create_table();

        for (ushort i = 0; i < 350; i++) {
            auto key = Util::getKeyStr(i);
            if (!key.empty()) {
                core["keys"][key] = i;
                core["keycodes"][i] = key;
            }
        }

        core.set_function("register_keybind", [&](sol::this_environment env, std::string identifier, sol::table data) {
            if (identifier.length() == 0) throw "Missing Identifier";
//            auto modname = static_cast<sol::environment>(env).get<std::string>("_MODNAME");
//            if (identifier.compare(0, modname.length() + 1, modname + ":")) {
//                std::cout << Log::err << "\"" << identifier << "\" does not begin with its mod's prefix \"" << modname << "\"!" << Log::endl;
//            }
            core["registered_keybinds"][identifier] = data;
        });
    }
}