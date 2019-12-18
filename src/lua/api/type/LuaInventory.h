//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include "../../../game/scene/world/Inventory.h"
#include "LuaInventoryList.h"

class LuaInventory {
public:
    LuaInventory(Inventory& inventory) : inventory(inventory), defs(inventory.defs) {}

    Inventory& inventory;
    DefinitionAtlas& defs;

    LuaInventoryList add_list(std::string name, int size, int width);
    LuaInventoryList get_list(std::string name);
    void remove_list(std::string name);
};
