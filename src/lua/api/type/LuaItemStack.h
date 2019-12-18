//
// Created by aurailus on 2019-12-17.
//

#pragma once

#include "../../../game/scene/world/ItemStack.h"
#include "../../../def/DefinitionAtlas.h"

class LuaItemStack {
public:
    LuaItemStack(const ItemStack& stack, const DefinitionAtlas& defs);
    LuaItemStack() = default;

    std::string get_name();
    void set_name(std::string name);

    int get_count();
    void set_count(int count);

    bool is_empty();

private:
    std::string name;
    unsigned short count;
};
