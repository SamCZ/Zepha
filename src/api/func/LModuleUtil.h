//
// Created by aurailus on 21/04/19.
//

#ifndef ZEUS_LMODULEUTIL_H
#define ZEUS_LMODULEUTIL_H

#include "../../def/block/graph/SelectionBox.h"
#include "../../def/block/BlockDef.h"
#include "../../def/block/BlockAtlas.h"
#include <sol.hpp>

class GameDefs;

class LModuleUtil {
public:
    LModuleUtil(sol::state& lua, sol::table& zeus, GameDefs& defs);
};



#endif //ZEUS_LMODULEUTIL_H
