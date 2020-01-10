//
// Created by aurailus on 2020-01-09.
//

#pragma once

#include <sol2/sol.hpp>
#include "../../../util/Log.h"

namespace Api {
    static void register_biome(sol::state& lua, sol::table& core) {
        core["registered_biomes"] = lua.create_table();

        core.set_function("register_biome", [&](sol::this_environment env, sol::optional<std::string> identifier, sol::optional<sol::table> data) {
            if (!identifier || !identifier->length()) throw "expected a string as the first argument.";
            if (!data) throw "expected a table as the second argument.";

            auto modname = static_cast<sol::environment>(env).get<std::string>("_MODNAME");
            if (identifier->compare(0, modname.length() + 1, modname + ":")) throw
                        "identifier '" + *identifier + "' must begin with the calling mod's prefix, '" + modname + "'.";

            core["registered_biomes"][identifier] = data;
        });
    }
}