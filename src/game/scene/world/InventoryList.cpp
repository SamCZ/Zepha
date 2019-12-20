//
// Created by aurailus on 2019-10-29.
//

#include <iostream>
#include <algorithm>
#include "InventoryList.h"

InventoryList::InventoryList(DefinitionAtlas& defs, unsigned short size, unsigned short width) :
    defs(defs),
    itemstacks(size),
    width(width) {}


void InventoryList::guiCallback(std::function<void()> cb) {
    this->cb = cb;
}

void InventoryList::registerLuaCallback(sol::function cb) {
    changeCallbacks.push_back(cb);
}

ItemStack InventoryList::getStack(unsigned short i) {
    return itemstacks[i];
}

void InventoryList::setStack(unsigned short i, const ItemStack &stack) {
    if (stack != getStack(i)) {
        itemstacks[i] = stack;
        triggerCallback();
    }
}

ItemStack InventoryList::placeStack(unsigned short i, const ItemStack &stack) {
    unsigned short maxStack = defs.fromId(stack.id).maxStackSize;

    auto otherStack = getStack(i);
    if (otherStack.id != stack.id) {
        setStack(i, stack);
        return otherStack;
    }
    else {
        unsigned short count = otherStack.count + stack.count;
        if (count <= maxStack) {
            setStack(i, {stack.id, count});
            return {0, 0};
        }
        else {
            setStack(i, {stack.id, maxStack});
            return {stack.id, static_cast<unsigned short>(count - maxStack)};
        }
    }
}

ItemStack InventoryList::splitStack(unsigned short i) {
    auto stack = getStack(i);
    unsigned short initialCount = stack.count;
    stack.count = floor(stack.count / 2.f);

    if (stack.count == 0) setStack(i, {});
    else setStack(i, stack);

    stack.count = ceil(initialCount / 2.f);
    return stack;
}

ItemStack InventoryList::addStack(ItemStack stack) {
    unsigned short maxStack = defs.fromId(stack.id).maxStackSize;

    unsigned short i = 0;
    while (i < itemstacks.size() && stack.count > 0) {
        if (itemstacks[i].count == 0) {
            if (stack.count <= maxStack) {
                itemstacks[i] = stack;
                triggerCallback();
                return ItemStack{};
            }
            else {
                itemstacks[i] = stack;
                itemstacks[i].count = maxStack;
                stack.count -= maxStack;
            }
        }
        else if (itemstacks[i].id == stack.id) {
            unsigned int fits = maxStack - itemstacks[i].count;
            if (fits >= stack.count) {
                itemstacks[i].count += stack.count;
                triggerCallback();
                return ItemStack {};
            }
            else {
                stack.count -= fits;
                itemstacks[i].count += fits;
            }
        }
        i++;
    }

    triggerCallback();
    return stack;
}

unsigned short InventoryList::stackFits(const ItemStack &stack) {
    unsigned short maxStack = defs.fromId(stack.id).maxStackSize;

    unsigned short i = 0;
    unsigned short fits = 0;

    while (i < itemstacks.size() && fits < stack.count) {
        if (itemstacks[i].count == 0) {
            fits += std::min(static_cast<unsigned short>(maxStack), stack.count);
        }
        else if (itemstacks[i].id == stack.id) {
            unsigned int canfit = maxStack - itemstacks[i].count;
            if (canfit >= stack.count - fits) {
                fits += stack.count - fits;
            }
            else {
                fits = stack.count;
            }
        }
        i++;
    }

    return fits;
}

ItemStack InventoryList::takeStack(ItemStack request) {
    unsigned short i = 0;
    unsigned short to_remove = request.count;

    while (i < itemstacks.size() && request.count > 0) {
        if (itemstacks[i].id == request.id) {
            unsigned int can_take = itemstacks[i].count;
            if (can_take >= to_remove) {
                itemstacks[i].count -= to_remove;
                triggerCallback();
                return request;
            }
            else {
                to_remove -= can_take;
                itemstacks[i] = ItemStack {};
            }
        }
        i++;
    }

    request.count = (request.count - to_remove);
    triggerCallback();
    return request;
}

ItemStack InventoryList::removeStack(unsigned short ind, unsigned short count) {
    auto stack = getStack(ind);
    if (count >= stack.count) {
        setStack(ind, {0, 0});
        return stack;
    }
    else {
        stack.count -= count;
        setStack(ind, stack);
        stack.count = count;
        return stack;
    }
}

void InventoryList::triggerCallback() {
    if (cb != nullptr) cb();
    for (auto& cb : changeCallbacks) {
        cb();
    }
}

unsigned short InventoryList::getLength() {
    return itemstacks.size();
}

unsigned short InventoryList::getWidth() {
    return width;
}