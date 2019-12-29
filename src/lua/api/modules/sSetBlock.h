//
// Created by aurailus on 28/06/19.
//

#pragma once

#include "../../../def/ServerDefs.h"
#include "../../../server/world/ServerWorld.h"
#include <sol2/sol.hpp>

namespace ServerApi {
    void set_block(sol::table &core, ServerDefs& defs, ServerWorld& world) {
        core.set_function("set_block", [&](sol::table pos, std::string identifier) {
            if (!pos["x"] || !pos["y"] || !pos["z"]) throw "expected a vector as the first argument.";
            world.setBlock({pos.get<float>("x"), pos.get<float>("y"), pos.get<float>("z")}, defs.defs.fromStr(identifier).index);
        });
    }
}
