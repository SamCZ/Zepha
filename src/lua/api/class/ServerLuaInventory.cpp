//
// Created by aurailus on 2019-12-17.
//

#include "ServerLuaInventory.h"

ServerLuaInventoryList ServerLuaInventory::add_list(std::string name, int size, int width) {
    inventory.createList(name, size, width);
    return ServerLuaInventoryList(*inventory[name]);
}

ServerLuaInventoryList ServerLuaInventory::get_list(std::string name) {
    return ServerLuaInventoryList(*inventory[name]);
}

void ServerLuaInventory::remove_list(std::string name) {
    inventory.removeList(name);
}
