//
// Created by aurailus on 2020-01-09.
//

#pragma once

#include "../../../def/DefinitionAtlas.h"
#include "../../../game/scene/world/World.h"

namespace Api {
    static void set_block(sol::table &core, DefinitionAtlas& defs, World& world) {
        core.set_function("set_block", [&](glm::vec3 pos, std::string identifier) {
            auto& block = defs.fromStr(identifier);
            world.setBlock(pos, block.index);
        });
    }
}