//
// Created by aurailus on 28/06/19.
//

#ifndef ZEUS_CGETBLOCK_H
#define ZEUS_CGETBLOCK_H

#include "../../../def/LocalDefs.h"
#include "../../../game/scene/world/LocalWorld.h"
#include <sol2/sol.hpp>

namespace ClientApi {
    void get_block(sol::table &zeus, LocalDefs& defs, LocalWorld& world) {
        zeus.set_function("get_block", [&](sol::table pos) -> std::string {
            if (!pos["x"] || !pos["y"] || !pos["z"]) {
                std::cout << Log::err << "get_block vector is ill formed." << Log::endl;
                return "";
            }
            return defs.defs().blockFromId(world.getBlock({pos["x"], pos["y"], pos["z"]})).identifier;
        });
    }
}

#endif //ZEUS_CGETBLOCK_H
