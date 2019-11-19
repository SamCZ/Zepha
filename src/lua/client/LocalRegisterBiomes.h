//
// Created by aurailus on 2019-11-18.
//

#pragma once

#include <sol2/sol.hpp>
#include "../../def/LocalDefs.h"

class LocalRegisterBiomes {
public:
    LocalRegisterBiomes(sol::table& core, LocalDefs& defs);
};
