//
// Created by aurailus on 2019-12-17.
//

#include "LuaInventory.h"

LuaInventoryList LuaInventory::add_list(std::string name, int size, int width) {
    inventory.createList(name, size, width);
    return LuaInventoryList(*inventory[name]);
}

LuaInventoryList LuaInventory::get_list(std::string name) {
    return LuaInventoryList(*inventory[name]);
}

void LuaInventory::remove_list(std::string name) {
    inventory.removeList(name);
}
