//
// Created by aurailus on 12/06/19.
//

#pragma once

#include <sol2/sol.hpp>

namespace ServerApi {
    void register_block(sol::state& lua, sol::table& core) {
        core["registered_blocks"] = lua.create_table();

        core.set_function("register_block", [&](sol::this_environment env, std::string identifier, sol::table data) {
            if (identifier.length() == 0) throw "Missing Identifier";
            auto modname = static_cast<sol::environment>(env).get<std::string>("_MODNAME");
            if (identifier.compare(0, modname.length() + 1, modname + ":")) {
                std::cout << Log::err << "\"" << identifier << "\" does not begin with its mod's prefix \"" << modname << "\"!" << Log::endl;
            }
            core["registered_blocks"][identifier] = data;
        });
    }
}
