//
// Created by aurailus on 2020-02-24.
//

#pragma once

#include "../../Lua.h"
#include "../../../game/inventory/LocalInventory.h"

class LocalLuaInventory {
public:
    LocalLuaInventory(LocalInventory& inventory) : inventory(inventory), defs(inventory.defs) {}

    LocalInventory& inventory;
    DefinitionAtlas& defs;

    sol::object get_list(sol::this_state s, std::string name);
};
