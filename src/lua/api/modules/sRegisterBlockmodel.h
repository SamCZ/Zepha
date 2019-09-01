//
// Created by aurailus on 28/06/19.
//

#pragma once

#include <sol2/sol.hpp>

namespace ServerApi {
    void register_blockmodel(sol::state& lua, sol::table& zeus) {
        zeus["registered_blockmodels"] = lua.create_table();

        zeus.set_function("register_blockmodel", [&](std::string identifier, sol::table data) {
            if (identifier.length() == 0) throw "Missing Identifier";
            zeus["registered_blockmodels"][identifier] = data;
        });
    }
}
