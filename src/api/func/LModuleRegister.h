//
// Created by aurailus on 18/12/18.
//

#ifndef ZEUS_LREGISTERBLOCK_H
#define ZEUS_LREGISTERBLOCK_H

#include "../../def/block/graph/SelectionBox.h"
#include <sol.hpp>

class GameDefs;

class LModuleRegister {
public:
    LModuleRegister(sol::state& lua, sol::table& zeus, GameDefs& defs);
};


#endif //ZEUS_LREGISTERBLOCK_H
