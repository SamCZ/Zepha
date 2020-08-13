//
// Created by aurailus on 2019-12-17.
//

#include "ItemStack.h"

#include "game/def/ItemDef.h"
#include "game/Subgame.h"
#include "game/atlas/DefinitionAtlas.h"
#include "lua/usertype/ItemStack.h"

ItemStack::ItemStack(Api::Usertype::ItemStack& stack, SubgamePtr game) :
    id((stack.get_count() == 0) ? DefinitionAtlas::AIR : game->getDefs().fromStr(stack.get_name()).index),
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
