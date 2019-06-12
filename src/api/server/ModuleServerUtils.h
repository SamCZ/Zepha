//
// Created by aurailus on 11/06/19.
//

#ifndef ZEUS_SMODULEUTILS_H
#define ZEUS_SMODULEUTILS_H

#include "../../def/block/SelectionBox.h"
#include "../../def/block/server/ServerBlockDef.h"
#include "../../def/block/server/ServerBlockAtlas.h"
#include <sol.hpp>

class ServerDefs;

class ModuleServerUtils {
public:
    ModuleServerUtils(sol::state& lua, sol::table& zeus, ServerDefs& defs);
};

#endif //ZEUS_SMODULEUTILS_H
