//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include <string>
#include "../../def/DefinitionAtlas.h"

class LuaItemStack;

class ItemStack {
public:
    ItemStack() = default;
    ItemStack(unsigned int id, unsigned short count);
    ItemStack(LuaItemStack& stack, const DefinitionAtlas& atlas);

    bool operator!=(const ItemStack& b) const;
    bool operator==(const ItemStack& b) const;

    unsigned int id = DefinitionAtlas::AIR;
    unsigned short count = 0;
};