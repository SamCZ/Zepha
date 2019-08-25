//
// Created by aurailus on 28/06/19.
//

#pragma once

#include "../../def/ServerDefs.h"
#include "../../server/world/ServerWorld.h"
#include <sol2/sol.hpp>

namespace ServerApi {
    void set_block(sol::table &zeus, ServerDefs& defs, ServerWorld& world) {
        zeus.set_function("set_block", [&](sol::table pos, std::string identifier) {
            if (!pos["x"] || !pos["y"] || !pos["z"]) {
                std::cout << Log::err << "set_block vector is ill formed." << Log::endl;
                return;
            }
            world.setBlock({pos["x"], pos["y"], pos["z"]}, defs.blocks().fromStr(identifier).index);
        });
    }
}
