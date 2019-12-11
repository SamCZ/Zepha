//
// Created by aurailus on 28/06/19.
//

#pragma once

#include "../../../def/ServerDefs.h"
#include "../../../server/world/ServerWorld.h"
#include <sol2/sol.hpp>

namespace ServerApi {
    void get_block(sol::table &core, ServerDefs& defs, ServerWorld& world) {
        core.set_function("get_block", [&](sol::table pos) -> std::string {
            if (!pos["x"] || !pos["y"] || !pos["z"]) {
                std::cout << Log::err << "get_block vector is ill formed." << Log::endl;
                return "";
            }
            return defs.defs.fromId(world.getBlock({pos["x"], pos["y"], pos["z"]})).identifier;
        });
    }
}
