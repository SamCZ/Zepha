//
// Created by aurailus on 28/06/19.
//

#ifndef ZEUS_CSETBLOCK_H
#define ZEUS_CSETBLOCK_H

#include "../../../def/LocalDefs.h"
#include "../../../game/scene/world/LocalWorld.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void set_block(sol::table &zeus, LocalDefs& defs, LocalWorld& world) {
        zeus.set_function("set_block", [&](sol::table pos, std::string identifier) {
            if (!pos["x"] || !pos["y"] || !pos["z"]) {
                std::cout << Log::err << "set_block vector is ill formed." << Log::endl;
                return;
            }
            world.setBlock({pos["x"], pos["y"], pos["z"]}, defs.blocks().fromIdentifier(identifier).index);
        });
    }
}

#endif //ZEUS_CSETBLOCK_H
