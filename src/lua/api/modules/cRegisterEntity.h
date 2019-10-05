//
// Created by aurailus on 04/10/19.
//

#pragma once

#include <sol2/sol.hpp>

namespace ClientApi {
    void register_entity(sol::state& lua, sol::table& core) {
        core["registered_entities"] = lua.create_table();

        core.set_function("register_entity", [&](std::string identifier, sol::table data) {
            if (identifier.length() == 0) throw "Missing Identifier";
            data["__index"] = data;
            core["registered_entities"][identifier] = data;
        });
    }
}
