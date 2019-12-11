//
// Created by aurailus on 28/06/19.
//

#pragma once

#include "../../../def/LocalDefs.h"
#include "../../../game/scene/world/LocalWorld.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void set_block(sol::table &core, LocalDefs& defs, LocalWorld& world) {
        core.set_function("set_block", [&](sol::table pos, std::string identifier) {
            if (!pos["x"] || !pos["y"] || !pos["z"]) {
                std::cout << Log::err << "set_block vector is ill formed." << Log::endl;
                return;
            }
            world.setBlock({pos["x"], pos["y"], pos["z"]}, defs.defs.blockFromStr(identifier).index);
        });
    }
}
