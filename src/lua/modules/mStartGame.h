//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include "lua/Lua.h"
#include "client/Client.h"
#include "util/net/Address.h"
#include "client/scene/ConnectScene.h"

namespace MenuApi {
    void start_game(Client& client, sol::table& core) {
        //TODO: Don't hardcode the subgame
        
        core.set_function("game_connect", [&](std::string address) {
            client.scene.setScene(std::make_unique<ConnectScene>(client, Address::fromString(address)));
        });

        core.set_function("game_host", [&](sol::this_state s) {
            sol::state_view state(s);
            const auto subgame = state.get<std::string>("zepha.__builtin.subgame");
            client.startLocalServer(subgame);
        });
    }
}