//
// Created by aurailus on 2019-11-12.
//

#pragma once

#include <sol2/sol.hpp>
#include "../../def/ServerDefs.h"
#include "../../def/gen/BiomeDef.h"

class ServerRegisterBiomes {
public:
    ServerRegisterBiomes(sol::table& core, ServerDefs& defs);
};
