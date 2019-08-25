//
// Created by aurailus on 28/06/19.
//

#pragma once

#include "../../def/LocalDefs.h"
#include "../../game/scene/world/LocalWorld.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void remove_block(sol::table &zeus, LocalDefs& defs, LocalWorld& world) {
        zeus.set_function("remove_block", [&](sol::table pos) {
            if (!pos["x"] || !pos["y"] || !pos["z"]) {
                std::cout << Log::err << "remove_block vector is ill formed." << Log::endl;
                return;
            }
            world.setBlock({pos["x"], pos["y"], pos["z"]}, LocalDefinitionAtlas::AIR);
        });
    }
}
