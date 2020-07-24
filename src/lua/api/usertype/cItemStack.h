//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include "../../Lua.h"
#include "../class/LuaItemStack.h"

namespace ClientApi {
    static void item_stack(sol::state& lua) {
        lua.new_usertype<LuaItemStack>("ItemStack",
            sol::constructors<LuaItemStack(sol::table), LuaItemStack(std::string, unsigned short)>(),

            "name", sol::property(&LuaItemStack::get_name, &LuaItemStack::set_name),
            "count", sol::property(&LuaItemStack::get_count, &LuaItemStack::set_count),

            "is_empty", &LuaItemStack::is_empty
        );
    }
}