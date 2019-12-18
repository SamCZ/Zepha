//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <sol2/sol.hpp>
#include "LuaItemStack.h"

namespace ClientApi {
    void item_stack(sol::state& lua) {
        lua.new_usertype<LuaItemStack>("ItemStack",
            "name", sol::property(&LuaItemStack::get_name, &LuaItemStack::set_name),
            "count", sol::property(&LuaItemStack::get_count, &LuaItemStack::set_count),

            "is_empty", &LuaItemStack::is_empty
        );
    }
}