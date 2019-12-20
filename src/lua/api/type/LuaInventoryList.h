//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include "../../../game/scene/world/InventoryList.h"
#include "LuaItemStack.h"

class LuaInventoryList {
public:
    LuaInventoryList(InventoryList& list) :
        list(list) {}

    InventoryList& list;

    int get_length();
    int get_width();

    LuaItemStack get_stack(unsigned short i);

    void set_stack(unsigned short i, LuaItemStack stack);
    void set_table(unsigned short i, sol::table stack);

    LuaItemStack place_stack(unsigned short i, LuaItemStack stack);
    LuaItemStack place_table(unsigned short i, sol::table stack);

    LuaItemStack split_stack(unsigned short i);

    LuaItemStack add_stack(LuaItemStack stack);
    LuaItemStack add_table(sol::table stack);

    int stack_fits(LuaItemStack stack);
    int table_fits(sol::table stack);

    LuaItemStack take_stack(LuaItemStack request);
    LuaItemStack take_table(sol::table request);

    LuaItemStack remove_stack(unsigned short ind, unsigned short count);

    void register_on_change(sol::function cb);
};
