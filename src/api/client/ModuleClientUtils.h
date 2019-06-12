//
// Created by aurailus on 21/04/19.
//

#ifndef ZEUS_LMODULEUTIL_H
#define ZEUS_LMODULEUTIL_H

#include "../../def/block/SelectionBox.h"
#include "../../def/block/client/LocalBlockDef.h"
#include "../../def/block/client/LocalBlockAtlas.h"
#include <sol.hpp>

class LocalDefs;

class ModuleClientUtils {
public:
    ModuleClientUtils(sol::state& lua, sol::table& zeus, LocalDefs& defs);
};

#endif //ZEUS_LMODULEUTIL_H
