//
// Created by aurailus on 2019-12-17.
//

#include "ItemStack.h"
#include "../../../lua/api/type/LuaItemStack.h"

ItemStack::ItemStack(LuaItemStack &stack, const DefinitionAtlas &atlas) :
    id(atlas.fromStr(stack.get_name()).index),
    count(stack.get_count()) {}

ItemStack::ItemStack(unsigned int id, unsigned short count) :
    id(id),
    count(count) {}

bool ItemStack::operator!=(const ItemStack &b) const {
    return (b.count != this->count || b.id != this->id);
}

bool ItemStack::operator==(const ItemStack &b) const {
    return (b.count == this->count && b.id == this->id);
}
