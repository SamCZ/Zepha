//
// Created by aurailus on 2019-12-17.
//

#include "ItemStack.h"
#include "../../lua/api/class/LuaItemStack.h"

ItemStack::ItemStack(LuaItemStack &stack, const DefinitionAtlas &atlas) :
    id((stack.get_count() == 0) ? DefinitionAtlas::AIR : atlas.fromStr(stack.get_name()).index),
    count((this->id == DefinitionAtlas::AIR) ? 0 : stack.get_count()) {}

ItemStack::ItemStack(unsigned int id, unsigned short count) :
    id((count == 0) ? DefinitionAtlas::AIR : id),
    count((this->id == DefinitionAtlas::AIR) ? 0 : count) {}

bool ItemStack::operator!=(const ItemStack &b) const {
    return (b.count != this->count || b.id != this->id);
}

bool ItemStack::operator==(const ItemStack &b) const {
    return (b.count == this->count && b.id == this->id);
}
