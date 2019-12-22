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
            "set_stack", sol::overload(
                    sol::resolve<void(unsigned short, LuaItemStack)>(&LuaInventoryList::set_stack),
                    sol::resolve<void(unsigned short, sol::table)>(&LuaInventoryList::set_stack)),
            "place_stack", sol::overload(
                    sol::resolve<LuaItemStack(unsigned short, LuaItemStack)>(&LuaInventoryList::place_stack),
                    sol::resolve<LuaItemStack(unsigned short, sol::table)>(&LuaInventoryList::place_stack)),
            "split_stack", &LuaInventoryList::split_stack,
            "add_stack", sol::overload(
                    sol::resolve<LuaItemStack(LuaItemStack)>(&LuaInventoryList::add_stack),
                    sol::resolve<LuaItemStack(sol::table)>(&LuaInventoryList::add_stack)),
            "stack_fits", sol::overload(
                    sol::resolve<int(LuaItemStack)>(&LuaInventoryList::stack_fits),
                    sol::resolve<int(sol::table)>(&LuaInventoryList::stack_fits)),
            "take_stack", sol::overload(
                    sol::resolve<LuaItemStack(LuaItemStack)>(&LuaInventoryList::take_stack),
                    sol::resolve<LuaItemStack(sol::table)>(&LuaInventoryList::take_stack)),
            "remove_stack", &LuaInventoryList::remove_stack,

            "on_put", sol::property(
                   [](LuaInventoryList l){ return l.get_callback(InventoryList::Callback::ON_PUT); },
                   [](LuaInventoryList l, sol::function f){ l.set_callback(InventoryList::Callback::ON_PUT, f); }),
            "on_take", sol::property(
                   [](LuaInventoryList l){ return l.get_callback(InventoryList::Callback::ON_TAKE); },
                   [](LuaInventoryList l, sol::function f){ l.set_callback(InventoryList::Callback::ON_TAKE, f); }),
            "allow_put", sol::property(
                   [](LuaInventoryList l){ return l.get_callback(InventoryList::Callback::ALLOW_PUT); },
                   [](LuaInventoryList l, sol::function f){ l.set_callback(InventoryList::Callback::ALLOW_PUT, f); }),
            "allow_take", sol::property(
                   [](LuaInventoryList l){ return l.get_callback(InventoryList::Callback::ALLOW_TAKE); },
                   [](LuaInventoryList l, sol::function f){ l.set_callback(InventoryList::Callback::ALLOW_TAKE, f); })
        );
    }
}