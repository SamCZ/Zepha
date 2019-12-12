//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include <sol2/sol.hpp>

namespace MenuApi {
    void printe(sol::state &lua) {
        lua.script(R"(
            function printe(str)
                print("\27[31m[!] " .. str .. "\27[0m")
            end
        )");
    }
}
