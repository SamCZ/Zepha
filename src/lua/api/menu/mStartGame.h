//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include "../../Lua.h"
#include "../../../game/ClientState.h"

namespace MenuApi {
    void start_game(ClientState& state, sol::table& core) {
        //TODO: Don't hardcode the subgame
        
        core.set_function("start_game", [&]() {
            state.subgame = "zeus";
            state.desiredState = "connect";
        });

        core.set_function("start_game_local", [&]() {
            state.subgame = "zeus";
            state.desiredState = "local";
        });
    }
}