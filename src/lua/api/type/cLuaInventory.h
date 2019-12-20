//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <sol2/sol.hpp>
#include "LuaInventory.h"

void remove_list(std::string name);
namespace ClientApi {
    void inventory(sol::state& lua) {
        lua.new_usertype<LuaInventory>("InventoryRef",
            "add_list", &LuaInventory::add_list,
            "get_list", &LuaInventory::get_list,
            "remove_list", &LuaInventory::remove_list
        );

        lua.new_usertype<LuaInventoryList>("InventoryListRef",
            "length", sol::property(&LuaInventoryList::get_length),
            "width", sol::property(&LuaInventoryList::get_width),

            "get_stack", &LuaInventoryList::get_stack,
            "set_stack", sol::overload(&LuaInventoryList::set_stack, &LuaInventoryList::set_table),

            "place_stack", sol::overload(&LuaInventoryList::place_stack, &LuaInventoryList::place_table),
            "split_stack", &LuaInventoryList::split_stack,

            "add_stack", sol::overload(&LuaInventoryList::add_stack, &LuaInventoryList::add_table),
            "stack_fits", sol::overload(&LuaInventoryList::stack_fits, &LuaInventoryList::table_fits),
            "take_stack", sol::overload(&LuaInventoryList::take_stack, &LuaInventoryList::take_table),

            "remove_stack", &LuaInventoryList::remove_stack,

            "register_on_change", &LuaInventoryList::register_on_change
        );
    }
}