//
// Created by aurailus on 28/06/19.
//

#ifndef ZEUS_SPRINTE_H
#define ZEUS_SPRINTE_H

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

#endif //ZEUS_CPRINTE_H
