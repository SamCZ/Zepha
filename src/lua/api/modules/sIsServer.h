//
// Created by aurailus on 28/06/19.
//

#pragma once

#include <sol2/sol.hpp>

namespace ServerApi {
    void is_server(sol::table& core) {
        core.set_function("is_server", []() {
            return true;
        });
    }
}