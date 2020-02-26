//
// Created by aurailus on 2020-02-24.
//

#pragma once

#include "LocalLuaInventoryList.h"
#include "../../../game/inventory/LocalInventory.h"

class LocalLuaInventory {
public:
    LocalLuaInventory(LocalInventory& inventory) : inventory(inventory), defs(inventory.defs) {}

    LocalInventory& inventory;
    DefinitionAtlas& defs;

//    LocalLuaInventoryList add_list(std::string name, int size, int width);
    LocalLuaInventoryList get_list(std::string name);
//    void remove_list(std::string name);
};
