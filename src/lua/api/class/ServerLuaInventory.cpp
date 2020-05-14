//
// Created by aurailus on 2019-12-17.
//

#include "ServerLuaInventory.h"

sol::object ServerLuaInventory::add_list(sol::this_state s, std::string name, int size, int width) {
    inventory.createList(name, size, width);
    return sol::make_object<ServerLuaInventoryList>(s, ServerLuaInventoryList(*inventory[name]));
}

sol::object ServerLuaInventory::get_list(sol::this_state s, std::string name) {
    auto inv = inventory[name];
    if (!inv) return sol::nil;
    return sol::make_object<ServerLuaInventoryList>(s, ServerLuaInventoryList(*inv));
}

void ServerLuaInventory::remove_list(std::string name) {
    inventory.removeList(name);
}
