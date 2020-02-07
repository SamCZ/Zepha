//
// Created by aurailus on 2019-10-29.
//

#include <iostream>
#include <algorithm>
#include "InventoryList.h"
#include "../../../lua/api/type/LuaItemStack.h"

InventoryList::InventoryList(DefinitionAtlas& defs, std::string name, unsigned short size, unsigned short width) :
    defs(defs),
    name(name),
    itemstacks(size),
    width(width) {}


void InventoryList::setGuiCallback(std::function<void()> cb) {
    this->guiCallback = cb;
}

void InventoryList::setLuaCallback(InventoryList::Callback type, sol::function cb) {
    luaCallbacks[static_cast<size_t>(type)] = cb;
}

sol::function InventoryList::getLuaCallback(InventoryList::Callback type) {
    return luaCallbacks[static_cast<size_t>(type)];
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

ItemStack InventoryList::placeStack(unsigned short i, const ItemStack &stack, bool playerInitiated) {
    auto otherStack = getStack(i);

    unsigned short allowedTake = otherStack.count;
    if (playerInitiated) {
        auto allowTake = luaCallbacks[static_cast<int>(Callback::ALLOW_TAKE)];
        if (allowTake) allowedTake = std::min(static_cast<unsigned short>(allowTake(i+1, LuaItemStack(otherStack, defs))), allowedTake);
    }
    unsigned short allowedPut = stack.count;
    if (playerInitiated) {
        auto allowPut = luaCallbacks[static_cast<int>(Callback::ALLOW_PUT)];
        if (allowPut) allowedPut = std::min(static_cast<unsigned short>(allowPut(i+1, LuaItemStack(stack, defs))), allowedPut);
    }

    sol::function on_put = luaCallbacks[static_cast<int>(Callback::ON_PUT)];
    sol::function on_take = luaCallbacks[static_cast<int>(Callback::ON_TAKE)];

    if (stack.count == 0) {
        if (allowedTake == otherStack.count) setStack(i, {});
        else setStack(i, {otherStack.id, static_cast<unsigned short>(otherStack.count - allowedTake)});
        if (allowedTake > 0 && on_take) on_take(i+1, LuaItemStack(otherStack, defs));
        return {otherStack.id, allowedTake};
    }
    else {
        if (otherStack.count) {
            if (otherStack.id == stack.id) {
                unsigned short maxStack = defs.fromId(stack.id).maxStackSize;
                if (allowedPut >= stack.count && allowedPut + otherStack.count < maxStack) {
                    setStack(i, {stack.id, static_cast<unsigned short>(otherStack.count + allowedPut)});
                    if (on_put) on_put(i+1, LuaItemStack(otherStack, defs));
                    return {};
                }
                else if (allowedPut > 0) {
                    allowedPut = std::min(allowedPut, static_cast<unsigned short>(maxStack - otherStack.count));
                    setStack(i, {stack.id, static_cast<unsigned short>(otherStack.count + allowedPut)});
                    if (allowedPut > 0 && on_put) on_put(i+1, LuaItemStack(otherStack, defs));
                    return {stack.id, static_cast<unsigned short>(stack.count - allowedPut)};
                }
                else {
                    allowedTake = std::min(allowedTake, static_cast<unsigned short>(maxStack - stack.count));
                    setStack(i, {stack.id, static_cast<unsigned short>(otherStack.count - allowedTake)});
                    if (allowedTake > 0 && on_take) on_take(i+1, LuaItemStack(otherStack, defs));
                    return {stack.id, static_cast<unsigned short>(stack.count + allowedTake)};
                }
            }
            else {
                if (stack.count <= allowedPut && otherStack.count <= allowedTake) {
                    setStack(i, stack);
                    if (allowedPut > 0 && on_put) on_put(i+1, LuaItemStack(otherStack, defs));
                    if (on_take) on_take(i+1, LuaItemStack(otherStack, defs));
                    return otherStack;
                }
                else {
                    return stack;
                }
            }
        }
        else {
            setStack(i, {stack.id, static_cast<unsigned short>(otherStack.count + allowedPut)});
            if (allowedPut > 0 && on_put) on_put(i+1, LuaItemStack(otherStack, defs));
            return {stack.id, static_cast<unsigned short>(stack.count - allowedPut)};
        }
    }
}

ItemStack InventoryList::splitStack(unsigned short i, bool playerInitiated) {
    auto stack = getStack(i);

    unsigned short allowedTake = stack.count;
    if (playerInitiated) {
        auto allowTake = luaCallbacks[static_cast<int>(Callback::ALLOW_TAKE)];
        if (allowTake) allowedTake = std::min(static_cast<unsigned short>(allowTake(i + 1, LuaItemStack(stack, defs))), allowedTake);
    }

    unsigned short initialCount = stack.count;
    unsigned short takeCount = std::min(static_cast<unsigned short>(ceil(initialCount / 2.f)), allowedTake);

    setStack(i, {stack.id, static_cast<unsigned short>(initialCount - takeCount)});
    sol::function on_take = luaCallbacks[static_cast<int>(Callback::ON_TAKE)];
    if (on_take) on_take(i+1, stack);
    return {stack.id, takeCount};
}

ItemStack InventoryList::addStack(ItemStack stack, bool playerInitiated) {
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

ItemStack InventoryList::takeStack(ItemStack request, bool playerInitiated) {
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
    if (guiCallback != nullptr) guiCallback();
}

unsigned short InventoryList::getLength() {
    return itemstacks.size();
}

unsigned short InventoryList::getWidth() {
    return width;
}

std::string InventoryList::getName() {
    return name;
}