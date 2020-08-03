//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include <string>

#include "../../util/CovariantPtr.h"
#include "../../lua/usertype/ItemStack.h"

class ItemStack {
public:
    ItemStack() = default;
    ItemStack(unsigned int id, unsigned short count);
    ItemStack(Api::Usertype::ItemStack& stack, SubgamePtr game);

    bool operator!=(const ItemStack& b) const;
    bool operator==(const ItemStack& b) const;

    unsigned int id = 0; // Invalid
    unsigned short count = 0;
};