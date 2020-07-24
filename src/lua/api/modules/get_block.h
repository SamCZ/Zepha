//
// Created by aurailus on 2020-01-09.
//

#pragma once

#include "../../../def/DefinitionAtlas.h"
#include "../../../game/scene/world/World.h"

namespace Api {
    static void get_block(sol::table &core, DefinitionAtlas& defs, World& world) {
        core.set_function("get_block", [&](glm::ivec3 pos) {
            return defs.fromId(world.getBlock(pos)).identifier;
        });
    }
}
