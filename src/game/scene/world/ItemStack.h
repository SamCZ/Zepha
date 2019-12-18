//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include <string>
#include "../../../def/DefinitionAtlas.h"

class LuaItemStack;

class ItemStack {
public:
    ItemStack() = default;
    ItemStack(unsigned int id, unsigned short count);
    ItemStack(LuaItemStack& stack, const DefinitionAtlas& atlas);

    unsigned int id = 0;
    unsigned short count = 0;
};