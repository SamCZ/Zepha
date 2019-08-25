//
// Created by aurailus on 28/06/19.
//

#pragma once

#include <sol2/sol.hpp>

namespace ServerApi {
    void printe(sol::state& lua) {
        lua.script(R"(
            function printe(str)
                print("\27[31m[!] " .. str .. "\27[0m")
            end
        )");
    }
}
