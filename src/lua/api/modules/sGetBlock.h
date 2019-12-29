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
            if (!pos["x"] || !pos["y"] || !pos["z"]) throw "expected a vector as the first argument.";
            return defs.defs.fromId(world.getBlock({pos.get<float>("x"), pos.get<float>("y"), pos.get<float>("z")})).identifier;
        });
    }
}
