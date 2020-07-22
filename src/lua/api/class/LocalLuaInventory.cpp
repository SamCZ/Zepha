//
// Created by aurailus on 2020-02-24.
//

#include "LocalLuaInventory.h"

#include "LocalLuaInventoryList.h"

sol::object LocalLuaInventory::get_list(sol::this_state s, std::string name) {
    auto list = inventory[name];
    if (!list) return sol::nil;
    return sol::make_object<LocalLuaInventoryList>(s, LocalLuaInventoryList(*list));
}