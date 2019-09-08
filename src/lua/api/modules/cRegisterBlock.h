//
// Created by aurailus on 12/06/19.
//

#pragma once

#include <sol2/sol.hpp>

namespace ClientApi {
    void register_block(sol::state& lua, sol::table& core) {
        core["registered_blocks"] = lua.create_table();

        core.set_function("register_block", [&](std::string identifier, sol::table data) {
            if (identifier.length() == 0) throw "Missing Identifier";
            core["registered_blocks"][identifier] = data;
        });
    }
}
