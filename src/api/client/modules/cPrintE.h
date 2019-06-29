//
// Created by aurailus on 28/06/19.
//

#ifndef ZEUS_CPRINTE_H
#define ZEUS_CPRINTE_H

#include <sol.hpp>

namespace ClientApi {
    void print_e(sol::state& lua) {
        lua.script(R"(
            function printe(str)
                print("\27[31m[!] " .. str .. "\27[0m")
            end
        )");
    }
}

#endif //ZEUS_CPRINTE_H
