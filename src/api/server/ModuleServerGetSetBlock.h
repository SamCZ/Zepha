//
// Created by aurailus on 12/06/19.
//

#ifndef ZEUS_MODULESERVERGETSETBLOCK_H
#define ZEUS_MODULESERVERGETSETBLOCK_H

#include "../../def/ServerDefs.h"
#include "../../server/world/ServerWorld.h"
#include <sol.hpp>

class ModuleServerGetSetBlock {
public:
    ModuleServerGetSetBlock(sol::state& lua, sol::table& zeus, ServerDefs& defs, ServerWorld& world) {

        zeus.set_function("get_block", [&](sol::table pos) -> std::string {
            if (!pos["x"] || !pos["y"] || !pos["z"]) {
                std::cout << Log::err << "get_block vector is ill formed." << Log::endl;
                return "";
            }
            return defs.blocks().fromIndex(world.getBlock({pos["x"], pos["y"], pos["z"]})).getIdentifier();
        });

        zeus.set_function("set_block", [&](sol::table pos, std::string identifier) {
            if (!pos["x"] || !pos["y"] || !pos["z"]) {
                std::cout << Log::err << "set_block vector is ill formed." << Log::endl;
                return;
            }
            world.setBlock({pos["x"], pos["y"], pos["z"]}, defs.blocks().fromIdentifier(identifier).getIndex());
        });

        zeus.set_function("remove_block", [&](sol::table pos) {
            if (!pos["x"] || !pos["y"] || !pos["z"]) {
                std::cout << Log::err << "remove_block vector is ill formed." << Log::endl;
                return;
            }
            world.setBlock({pos["x"], pos["y"], pos["z"]}, defs.blocks().fromIdentifier("builtin:air").getIndex());
        });
    }
};

#endif //ZEUS_MODULESERVERGETSETBLOCK_H
