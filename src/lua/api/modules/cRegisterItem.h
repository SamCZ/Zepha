//
// Created by aurailus on 13/08/19.
//

#pragma once

#include <sol2/sol.hpp>

namespace ClientApi {
    void register_item(sol::state& lua, sol::table& core) {
        core["registered_items"] = lua.create_table();

        core.set_function("register_item", [&](std::string identifier, sol::table data) {
            if (identifier.length() == 0) throw "Missing Identifier";
            core["registered_items"][identifier] = data;
        });
    }
}
