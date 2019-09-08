//
// Created by aurailus on 28/06/19.
//

#pragma once

#include "../../../def/ServerDefs.h"
#include "../../../server/world/ServerWorld.h"
#include <sol2/sol.hpp>

namespace ServerApi {
    void remove_block(sol::table &core, ServerDefs& defs, ServerWorld& world) {
        core.set_function("remove_block", [&](sol::table pos) {
            if (!pos["x"] || !pos["y"] || !pos["z"]) {
                std::cout << Log::err << "remove_block vector is ill formed." << Log::endl;
                return;
            }
            world.setBlock({pos["x"], pos["y"], pos["z"]}, LocalDefinitionAtlas::AIR);
        });
    }
}
