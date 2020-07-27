//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include "../../game/inventory/Inventory.h"

class ServerLuaInventory {
public:
    ServerLuaInventory(Inventory& inventory) : inventory(inventory), defs(inventory.defs) {}

    Inventory& inventory;
    DefinitionAtlas& defs;

    sol::object add_list(sol::this_state s, std::string name, int size, int width);
    sol::object get_list(sol::this_state s, std::string name);
    void remove_list(std::string name);

    void set_default_list(sol::object list);
    sol::object get_default_list(sol::this_state s);
};
