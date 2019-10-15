//
// Created by aurailus on 12/10/19.
//

#pragma once

#include "../../def/LocalDefs.h"

class LocalRegisterKeybinds {
public:
    LocalRegisterKeybinds(sol::table& core, LocalDefs &defs, LuaInputManager& manager);
};
