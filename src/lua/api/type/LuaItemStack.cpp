//
// Created by aurailus on 2019-12-17.
//

#include "LuaItemStack.h"

LuaItemStack::LuaItemStack(const ItemStack &stack, const DefinitionAtlas &defs) :
    name(defs.fromId(stack.id).name),
    count(stack.count) {}

std::string LuaItemStack::get_name() {
    return name;
}

void LuaItemStack::set_name(std::string name) {
    this->name = name;
}

int LuaItemStack::get_count() {
    return count;
}

void LuaItemStack::set_count(int count) {
    this->count = count;
}

bool LuaItemStack::is_empty() {
    return count == 0;
}
