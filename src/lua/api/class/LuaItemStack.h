//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include <sol2/sol.hpp>

class DefinitionAtlas;
class ItemStack;

class LuaItemStack {
public:
    LuaItemStack() = default;
    LuaItemStack(const ItemStack& stack, const DefinitionAtlas& defs);
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
