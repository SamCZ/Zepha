//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include "../../../game/scene/world/Inventory.h"
#include "ServerLuaInventoryList.h"

class ServerLuaInventory {
public:
    ServerLuaInventory(Inventory& inventory) : inventory(inventory), defs(inventory.defs) {}

    Inventory& inventory;
    DefinitionAtlas& defs;

    ServerLuaInventoryList add_list(std::string name, int size, int width);
    ServerLuaInventoryList get_list(std::string name);
    void remove_list(std::string name);
};
