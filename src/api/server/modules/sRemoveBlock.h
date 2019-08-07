//
// Created by aurailus on 28/06/19.
//

#ifndef ZEUS_SREMOVEBLOCK_H
#define ZEUS_SREMOVEBLOCK_H

#include "../../../def/ServerDefs.h"
#include "../../../server/world/ServerWorld.h"
#include <sol2/sol.hpp>

namespace ServerApi {
    void remove_block(sol::table &zeus, ServerDefs& defs, ServerWorld& world) {
        zeus.set_function("remove_block", [&](sol::table pos) {
            if (!pos["x"] || !pos["y"] || !pos["z"]) {
                std::cout << Log::err << "remove_block vector is ill formed." << Log::endl;
                return;
            }
            world.setBlock({pos["x"], pos["y"], pos["z"]}, LocalBlockAtlas::AIR);
        });
    }
}

#endif //ZEUS_CREMOVEBLOCK_H
