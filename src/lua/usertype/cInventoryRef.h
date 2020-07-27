//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include "../Lua.h"
#include "LocalLuaInventory.h"
#include "LocalLuaInventoryList.h"

namespace ClientApi {
    static void inventory(sol::state& lua) {
        lua.new_usertype<LocalLuaInventory>("InventoryRef",
            "get_list", &LocalLuaInventory::get_list
        );

        lua.new_usertype<LocalLuaInventoryList>("InventoryListRef",
            "length", sol::property(&LocalLuaInventoryList::get_length),
            "width", sol::property(&LocalLuaInventoryList::get_width),
            "name", sol::property(&LocalLuaInventoryList::get_name),

            "get_stack", &LocalLuaInventoryList::get_stack,
//            "set_stack", sol::overload(
//                sol::resolve<void(unsigned short, LuaItemStack)>(&LocalLuaInventoryList::set_stack),
//                sol::resolve<void(unsigned short, sol::table)>(&LocalLuaInventoryList::set_stack)),
//            "place_stack", sol::overload(
//                sol::resolve<LuaItemStack(unsigned short, LuaItemStack)>(&LocalLuaInventoryList::place_stack),
//                sol::resolve<LuaItemStack(unsigned short, sol::table)>(&LocalLuaInventoryList::place_stack)),
//            "split_stack", &LocalLuaInventoryList::split_stack,
//            "add_stack", sol::overload(
//                sol::resolve<LuaItemStack(LuaItemStack)>(&LocalLuaInventoryList::add_stack),
//                sol::resolve<LuaItemStack(sol::table)>(&LocalLuaInventoryList::add_stack)),
//            "stack_fits", sol::overload(
//                sol::resolve<int(LuaItemStack)>(&LocalLuaInventoryList::stack_fits),
//                sol::resolve<int(sol::table)>(&LocalLuaInventoryList::stack_fits)),
//            "take_stack", sol::overload(
//                sol::resolve<LuaItemStack(LuaItemStack)>(&LocalLuaInventoryList::take_stack),
//                sol::resolve<LuaItemStack(sol::table)>(&LocalLuaInventoryList::take_stack)),
//            "remove_stack", &LocalLuaInventoryList::remove_stack,

            "on_put", sol::property(
               [](LocalLuaInventoryList l){ return l.get_callback(LocalInventoryList::Callback::ON_PUT); },
               [](LocalLuaInventoryList l, sol::function f){ l.set_callback(LocalInventoryList::Callback::ON_PUT, f); }),
            "on_take", sol::property(
               [](LocalLuaInventoryList l){ return l.get_callback(LocalInventoryList::Callback::ON_TAKE); },
               [](LocalLuaInventoryList l, sol::function f){ l.set_callback(LocalInventoryList::Callback::ON_TAKE, f); }),
            "allow_put", sol::property(
               [](LocalLuaInventoryList l){ return l.get_callback(LocalInventoryList::Callback::ALLOW_PUT); },
               [](LocalLuaInventoryList l, sol::function f){ l.set_callback(LocalInventoryList::Callback::ALLOW_PUT, f); }),
            "allow_take", sol::property(
               [](LocalLuaInventoryList l){ return l.get_callback(LocalInventoryList::Callback::ALLOW_TAKE); },
               [](LocalLuaInventoryList l, sol::function f){ l.set_callback(LocalInventoryList::Callback::ALLOW_TAKE, f); })
        );
    }
}