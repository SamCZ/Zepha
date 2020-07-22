//
// Created by aurailus on 2019-12-17.
//

#include "ServerLuaInventory.h"

#include "ServerLuaInventoryList.h"

sol::object ServerLuaInventory::add_list(sol::this_state s, std::string name, int size, int width) {
    inventory.createList(name, size, width);
    return sol::make_object<ServerLuaInventoryList>(s, ServerLuaInventoryList(*inventory[name]));
}

sol::object ServerLuaInventory::get_list(sol::this_state s, std::string name) {
    auto list = inventory[name];
    if (!list) return sol::nil;
    return sol::make_object<ServerLuaInventoryList>(s, ServerLuaInventoryList(*list));
}

void ServerLuaInventory::remove_list(std::string name) {
    inventory.removeList(name);
}

void ServerLuaInventory::set_default_list(sol::object list) {
    if (list.is<std::string>()) inventory.setDefaultList(list.as<std::string>());
    else inventory.setDefaultList(list.as<sol::table>().get<std::string>("name"));
}

sol::object ServerLuaInventory::get_default_list(sol::this_state s) {
    if (inventory.getDefaultList() == "") return sol::nil;
    else get_list(s, inventory.getDefaultList());
}
