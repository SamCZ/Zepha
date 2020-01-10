//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include <iostream>
#include <sol2/sol.hpp>
#include "../../../game/ClientState.h"

namespace MenuApi {
    void start_game(ClientState& state, sol::table& core) {
        core.set_function("start_game", [&]() {
            state.desiredState = "connect";
        });
    }
}
