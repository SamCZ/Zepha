//
// Created by aurailus on 29/06/19.
//

#pragma once


#include <sol2/sol.hpp>
#include "../../def/ServerDefs.h"
#include "../../def/block/BlockModel.h"

class ServerRegisterBlocks {
public:
    ServerRegisterBlocks(sol::table& core, ServerDefs& defs);
};

