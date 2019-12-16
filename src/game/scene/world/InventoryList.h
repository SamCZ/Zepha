//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include <functional>
#include <vector>
#include "ItemStack.h"

class InventoryList {
public:
    InventoryList() = default;
    InventoryList(unsigned short size, unsigned short width);

    unsigned short getLength();
    unsigned short getWidth();

    ItemStack getStack(unsigned short i);
    void setStack(unsigned short i, const ItemStack& stack);

    //Place the stack at i into the existing stack, returning overflow or other stack.
    ItemStack placeStack(unsigned short i, const ItemStack& stack);
    //Split the stack at i and return half of it, rounded up
    ItemStack splitStack(unsigned short i);

    //Adds as much of `stack` to the inventory as possible, returns leftover.
    ItemStack addStack(ItemStack stack);
    //Returns how many of `stack`'s items fit, from 0 to stack.count.
    unsigned short stackFits(ItemStack& stack);
    //Attempts to remove the type and amount of items specified by `stack`. Returns actual ItemStack of what was removed.
    ItemStack removeStack(ItemStack request);

    void setUpdatedCallback(std::function<void()> cb);
    void clearUpdatedCallback();

    const static unsigned short MAX_STACK = 64; //TODO: Don't hardcode this.
private:
    std::vector<ItemStack> itemstacks {};
    unsigned short width = 0;

    std::function<void()> cb = nullptr;
    void triggerCallback();
};
