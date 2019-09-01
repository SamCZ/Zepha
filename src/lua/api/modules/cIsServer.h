//
// Created by aurailus on 28/06/19.
//

#pragma once

#include <sol2/sol.hpp>

namespace ClientApi {
    void is_server(sol::table& zeus) {
        zeus.set_function("is_server", []() {
            return false;
        });
    }
}
