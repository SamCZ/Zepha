//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include <list>
#include <vector>
#include <functional>
#include "ItemStack.h"

class InventoryList {
public:
    enum class Callback { TAKE, PUT };

    InventoryList(DefinitionAtlas& defs, unsigned short size, unsigned short width);

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
    unsigned short stackFits(const ItemStack& stack);
    //Attempts to remove the type and amount of items specified by `stack`. Returns actual ItemStack of what was removed.
    ItemStack takeStack(ItemStack request);

    //Removes up to count items from ind, returns the items removed
    ItemStack removeStack(unsigned short ind, unsigned short count);

    void guiCallback(std::function<void()> cb);
    void registerLuaCallback(sol::function cb);

    DefinitionAtlas& defs;

private:
    std::vector<ItemStack> itemstacks {};
    unsigned short width = 0;

    std::function<void()> cb = nullptr;
    void triggerCallback();

    std::list<sol::function> changeCallbacks {};
};
