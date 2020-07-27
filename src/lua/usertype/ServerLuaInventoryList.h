//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include "LuaItemStack.h"
#include "../../game/inventory/ServerInventoryList.h"

class ServerLuaInventoryList {
public:
    ServerLuaInventoryList(InventoryList& list) :
        list(list) {}

    InventoryList& list;

    void set_length(int length);
    int get_length();

    void set_width(int width);
    int get_width();

    void resize(int length, int width);

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

    void set_callback(ServerInventoryList::Callback t, sol::safe_function fun);
    sol::safe_function get_callback(ServerInventoryList::Callback t);
};
