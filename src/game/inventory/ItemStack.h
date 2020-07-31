//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include <string>

class Subgame;
class LuaItemStack;

class ItemStack {
public:
    ItemStack() = default;
    ItemStack(unsigned int id, unsigned short count);
    ItemStack(LuaItemStack& stack, Subgame& game);

    bool operator!=(const ItemStack& b) const;
    bool operator==(const ItemStack& b) const;

    unsigned int id = 0; // Invalid
    unsigned short count = 0;
};