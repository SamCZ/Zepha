//
// Created by aurailus on 2020-03-04.
//

#pragma once

#include <sol2/sol.hpp>
#include "../../../util/Timer.h"

namespace Api {
    static void time(sol::state& lua, sol::table& core) {
        core["time"] = lua.create_table();
        sol::table time = core["time"];

        Timer t = Timer("timer");

        time.set_function("ns", [=]() {
            return t.elapsedNs();
        });

        time.set_function("ms", [=]() {
            return t.elapsedNs() / 1000000.f;
        });

        time.set_function("s", [=]() {
            return t.elapsedNs() / 1000000.f / 1000.f;
        });
    }
}