//
// Created by aurailus on 28/06/19.
//

#pragma once

#include "../../../def/LocalDefs.h"
#include "../../../game/scene/world/LocalWorld.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void get_block(sol::table &core, LocalDefs& defs, LocalWorld& world) {
        core.set_function("get_block", [&](sol::table pos) -> std::string {
            if (!pos["x"] || !pos["y"] || !pos["z"]) {
                std::cout << Log::err << "get_block vector is ill formed." << Log::endl;
                return "";
            }
            return defs.defs.blockFromId(world.getBlock({pos["x"], pos["y"], pos["z"]})).identifier;
        });
    }
}
