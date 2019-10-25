//
// Created by aurailus on 01/08/19.
//

#pragma once

#include "../../../def/LocalDefs.h"
#include "../../../game/scene/world/Player.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void close_menu(sol::table &core, LocalDefs& defs, Player& player) {

        core.set_function("close_menu", [&]() {
            player.closeMenu();
        });
    }
}

