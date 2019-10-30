//
// Created by aurailus on 2019-10-29.
//

#include <iostream>
#include "InventoryList.h"

InventoryList::InventoryList(unsigned short size, unsigned short width) :
    itemstacks(size),
    width(width) {

    std::cout << itemstacks.size() << std::endl;
}


void InventoryList::setUpdatedCallback(std::function<void()> cb) {
    this->cb = cb;
}

ItemStack InventoryList::getStack(unsigned short i) {
    return itemstacks[i];
}

void InventoryList::setStack(unsigned short i, ItemStack &stack) {
    itemstacks[i] = stack;
    triggerCallback();
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
                itemstacks[i].count = 64;
                stack.count -= 64;
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

unsigned short InventoryList::stackFits(ItemStack &stack) {
    unsigned short i = 0;
    unsigned short fits = 0;

    while (i < itemstacks.size() && fits < stack.count) {
        if (itemstacks[i].count == 0) {
            fits += std::min(static_cast<unsigned short>(64), stack.count);
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
