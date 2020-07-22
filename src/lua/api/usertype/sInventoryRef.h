//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <sol2/sol.hpp>

#include "../class/ServerLuaInventory.h"
#include "../class/ServerLuaInventoryList.h"

namespace ServerApi {
    static void inventory(sol::state& lua) {
        lua.new_usertype<ServerLuaInventory>("InventoryRef",
            "get_list", &ServerLuaInventory::get_list,
            "add_list", &ServerLuaInventory::add_list,
            "remove_list", &ServerLuaInventory::remove_list,

            "set_default_list", &ServerLuaInventory::set_default_list,
            "get_default_list", &ServerLuaInventory::get_default_list
        );

        lua.new_usertype<ServerLuaInventoryList>("InventoryListRef",
            "length", sol::property(&ServerLuaInventoryList::get_length, &ServerLuaInventoryList::set_length),
            "width", sol::property(&ServerLuaInventoryList::get_width, &ServerLuaInventoryList::set_width),
            "name", sol::property(&ServerLuaInventoryList::get_name),

            "resize", &ServerLuaInventoryList::resize,

            "get_stack", &ServerLuaInventoryList::get_stack,
            "set_stack", sol::overload(
                sol::resolve<void(unsigned short, LuaItemStack)>(&ServerLuaInventoryList::set_stack),
                sol::resolve<void(unsigned short, sol::table)>(&ServerLuaInventoryList::set_stack)),
            "place_stack", sol::overload(
                sol::resolve<LuaItemStack(unsigned short, LuaItemStack)>(&ServerLuaInventoryList::place_stack),
                sol::resolve<LuaItemStack(unsigned short, sol::table)>(&ServerLuaInventoryList::place_stack)),
            "split_stack", &ServerLuaInventoryList::split_stack,
            "add_stack", sol::overload(
                sol::resolve<LuaItemStack(LuaItemStack)>(&ServerLuaInventoryList::add_stack),
                sol::resolve<LuaItemStack(sol::table)>(&ServerLuaInventoryList::add_stack)),
            "stack_fits", sol::overload(
                sol::resolve<int(LuaItemStack)>(&ServerLuaInventoryList::stack_fits),
                sol::resolve<int(sol::table)>(&ServerLuaInventoryList::stack_fits)),
            "take_stack", sol::overload(
                sol::resolve<LuaItemStack(LuaItemStack)>(&ServerLuaInventoryList::take_stack),
                sol::resolve<LuaItemStack(sol::table)>(&ServerLuaInventoryList::take_stack)),
            "remove_stack", &ServerLuaInventoryList::remove_stack,

            "on_put", sol::property(
                [](ServerLuaInventoryList l){ return l.get_callback(ServerInventoryList::Callback::ON_PUT); },
                [](ServerLuaInventoryList l, sol::function f){ l.set_callback(ServerInventoryList::Callback::ON_PUT, f); }),
            "on_take", sol::property(
                [](ServerLuaInventoryList l){ return l.get_callback(ServerInventoryList::Callback::ON_TAKE); },
                [](ServerLuaInventoryList l, sol::function f){ l.set_callback(ServerInventoryList::Callback::ON_TAKE, f); }),
            "allow_put", sol::property(
                [](ServerLuaInventoryList l){ return l.get_callback(ServerInventoryList::Callback::ALLOW_PUT); },
                [](ServerLuaInventoryList l, sol::function f){ l.set_callback(ServerInventoryList::Callback::ALLOW_PUT, f); }),
            "allow_take", sol::property(
                [](ServerLuaInventoryList l){ return l.get_callback(ServerInventoryList::Callback::ALLOW_TAKE); },
                [](ServerLuaInventoryList l, sol::function f){ l.set_callback(ServerInventoryList::Callback::ALLOW_TAKE, f); })
        );
    }
}
