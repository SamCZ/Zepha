//
// Created by aurailus on 28/06/19.
//

#pragma once

#include <sol2/sol.hpp>

namespace ClientApi {
    void register_blockmodel(sol::state& lua, sol::table& core) {
        core["registered_blockmodels"] = lua.create_table();

        core.set_function("register_blockmodel", [&](std::string identifier, sol::table data) {
            if (identifier.length() == 0) throw "Missing Identifier";
            core["registered_blockmodels"][identifier] = data;
        });
    }
}
