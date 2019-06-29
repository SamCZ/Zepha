//
// Created by aurailus on 28/06/19.
//

#ifndef ZEUS_SGETBLOCK_H
#define ZEUS_SGETBLOCK_H

#include "../../../def/ServerDefs.h"
#include "../../../server/world/ServerWorld.h"
#include <sol.hpp>

namespace ServerApi {
    void get_block(sol::table &zeus, ServerDefs& defs, ServerWorld& world) {
        zeus.set_function("get_block", [&](sol::table pos) -> std::string {
            if (!pos["x"] || !pos["y"] || !pos["z"]) {
                std::cout << Log::err << "get_block vector is ill formed." << Log::endl;
                return "";
            }
            return defs.blocks().fromIndex(world.getBlock({pos["x"], pos["y"], pos["z"]})).getIdentifier();
        });
    }
}

#endif //ZEUS_CGETBLOCK_H
