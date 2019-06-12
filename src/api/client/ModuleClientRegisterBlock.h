//
// Created by aurailus on 18/12/18.
//

#ifndef ZEUS_LREGISTERBLOCK_H
#define ZEUS_LREGISTERBLOCK_H

#include "../../def/block/SelectionBox.h"
#include "../../def/block/client/LocalBlockDef.h"
#include "../../def/block/client/LocalBlockAtlas.h"
#include <sol.hpp>

class LocalDefs;

class ModuleClientRegisterBlock {
public:
    ModuleClientRegisterBlock(sol::state& lua, sol::table& zeus, LocalDefs& defs);
};


#endif //ZEUS_LREGISTERBLOCK_H
