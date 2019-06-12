//
// Created by aurailus on 11/06/19.
//

#ifndef ZEUS_MODULESERVERREGISTERBLOCK_H
#define ZEUS_MODULESERVERREGISTERBLOCK_H

#include "../../def/block/SelectionBox.h"
#include "../../def/block/client/LocalBlockDef.h"
#include "../../def/block/client/LocalBlockAtlas.h"
#include <sol.hpp>

class ServerDefs;

class ModuleServerRegisterBlock {
public:
    ModuleServerRegisterBlock(sol::state& lua, sol::table& zeus, ServerDefs& defs);
};


#endif //ZEUS_MODULESERVERREGISTERBLOCK_H
