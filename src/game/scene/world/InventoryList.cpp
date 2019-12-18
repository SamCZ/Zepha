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


void InventoryList::setUpdatedCallback(std::function<void()> cb) {
    this->cb = cb;
}

void InventoryList::clearUpdatedCallback() {
    cb = nullptr;
}

ItemStack InventoryList::getStack(unsigned short i) {
    return itemstacks[i];
}

void InventoryList::setStack(unsigned short i, const ItemStack &stack) {
    itemstacks[i] = stack;
    triggerCallback();
}

ItemStack InventoryList::placeStack(unsigned short i, const ItemStack &stack) {
    auto otherStack = getStack(i);
    if (otherStack.id != stack.id) {
        setStack(i, stack);
        return otherStack;
    }
    else {
        unsigned short count = otherStack.count + stack.count;
        if (count <= MAX_STACK) {
            setStack(i, {stack.id, count});
            return {0, 0};
        }
        else {
            setStack(i, {stack.id, MAX_STACK});
            return {stack.id, static_cast<unsigned short>(count - MAX_STACK)};
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
    unsigned short i = 0;
    while (i < itemstacks.size() && stack.count > 0) {
        if (itemstacks[i].count == 0) {
            if (stack.count <= MAX_STACK) {
                itemstacks[i] = stack;
                triggerCallback();
                return ItemStack{};
            }
            else {
                itemstacks[i] = stack;
                itemstacks[i].count = MAX_STACK;
                stack.count -= MAX_STACK;
            }
        }
        else if (itemstacks[i].id == stack.id) {
            unsigned int fits = MAX_STACK - itemstacks[i].count;
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
    unsigned short i = 0;
    unsigned short fits = 0;

    while (i < itemstacks.size() && fits < stack.count) {
        if (itemstacks[i].count == 0) {
            fits += std::min(static_cast<unsigned short>(MAX_STACK), stack.count);
        }
        else if (itemstacks[i].id == stack.id) {
            unsigned int canfit = MAX_STACK - itemstacks[i].count;
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

ItemStack InventoryList::removeStack(ItemStack request) {
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

void InventoryList::triggerCallback() {
    if (cb != nullptr) cb();
}

unsigned short InventoryList::getLength() {
    return itemstacks.size();
}

unsigned short InventoryList::getWidth() {
    return width;
}
