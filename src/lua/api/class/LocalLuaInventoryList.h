//
// Created by aurailus on 2020-02-24.
//

#pragma once

#include "../../../game/inventory/LocalInventoryList.h"

class LocalLuaInventoryList {
public:
    LocalLuaInventoryList(LocalInventoryList& list) :
    list(list) {}

    LocalInventoryList& list;

    int get_length();
    int get_width();
    std::string get_name();

    LuaItemStack get_stack(unsigned short i);

    void set_stack(unsigned short i, LuaItemStack stack);
    void set_stack(unsigned short i, sol::table stack);

    LuaItemStack place_stack(unsigned short i, LuaItemStack stack);
    LuaItemStack place_stack(unsigned short i, sol::table stack);

    LuaItemStack split_stack(unsigned short i);

    LuaItemStack add_stack(LuaItemStack stack);
    LuaItemStack add_stack(sol::table stack);

    int stack_fits(LuaItemStack stack);
    int stack_fits(sol::table stack);

    LuaItemStack take_stack(LuaItemStack request);
    LuaItemStack take_stack(sol::table request);

    LuaItemStack remove_stack(unsigned short ind, unsigned short count);

    void set_callback(LocalInventoryList::Callback t, sol::protected_function fun);
    sol::protected_function get_callback(LocalInventoryList::Callback t);
};
