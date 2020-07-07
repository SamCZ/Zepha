//
// Created by aurailus on 2020-01-09.
//

#pragma once

#include "../../../def/DefinitionAtlas.h"
#include "../../../game/scene/world/World.h"

namespace Api {
    static void remove_block(sol::table &core, DefinitionAtlas& defs, World& world) {
        core.set_function("remove_block", [&](sol::table pos) {
            if (!pos["x"] || !pos["y"] || !pos["z"]) throw "expected a vector as the first argument.";
            world.setBlock({pos.get<float>("x"), pos.get<float>("y"), pos.get<float>("z")}, DefinitionAtlas::AIR);
        });
    }
}