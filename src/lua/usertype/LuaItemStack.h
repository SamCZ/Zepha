//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <string>
#include <sol/forward.hpp>

class Subgame;
class ItemStack;

class LuaItemStack {
public:
    LuaItemStack() = default;
    LuaItemStack(const ItemStack& stack, Subgame& game);
    LuaItemStack(const std::string& name, unsigned short count);
    LuaItemStack(sol::table tbl);

    std::string get_name();
    void set_name(std::string name);

    int get_count();
    void set_count(int count);

    bool is_empty();
private:
    std::string name;
    unsigned short count;
};
