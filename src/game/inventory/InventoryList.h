//
// Created by aurailus on 2020-02-28.
//

#pragma once

#include <string>
#include <sol2/sol.hpp>

#include "ItemStack.h"

class DefinitionAtlas;

class InventoryList {
public:
    enum class Callback {
        ALLOW_TAKE,
        ALLOW_PUT,
        ON_TAKE,
        ON_PUT
    };

    InventoryList(DefinitionAtlas& defs);
    InventoryList(DefinitionAtlas& defs, const std::string& invName, const std::string& listName, unsigned short size, unsigned short width);

    void setLength(unsigned short length);
    unsigned short getLength() const;

    void setWidth(unsigned short width);
    unsigned short getWidth() const;

    std::string getName() const;

    // Place the stack at i into the existing stack, returning overflow or other stack.
    virtual ItemStack placeStack(unsigned short i, const ItemStack& stack, bool playerInitiated = false);
    // Split the stack at i and return half of it, rounded up
    virtual ItemStack splitStack(unsigned short i, bool playerInitiated = false);
    // Adds as much of `stack` to the inventory as possible, returns leftover.
    virtual ItemStack addStack(ItemStack stack, bool playerInitiated = false);
    // Returns how many of `stack`'s items fit, from 0 to stack.count.
    virtual unsigned short stackFits(const ItemStack& stack);
    // Attempts to remove the usertype and amount of items specified by `stack`. Returns actual ItemStack of what was removed.
    virtual ItemStack takeStack(ItemStack request, bool playerInitiated = false);
    // Removes up to count items from ind, returns the items removed
    virtual ItemStack removeStack(unsigned short ind, unsigned short count);

    // Primary interaction - The action performed when left clicking an inventory slot.
    virtual void primaryInteract(InventoryList& cursor, unsigned short ind);
    // Secondary interaction - The action performed when right clicking an inventory slot.
    virtual void secondaryInteract(InventoryList& cursor, unsigned short ind);

    ItemStack getStack(unsigned short i) const;
    void setStack(unsigned short i, const ItemStack& stack);

    sol::function getLuaCallback(Callback type);
    void setLuaCallback(Callback type, sol::function cb);

    DefinitionAtlas& defs;

protected:
    virtual void manipulated() = 0;
    void initialize();

    std::string invName {}, listName {};
    unsigned short width = 0;

    std::vector<ItemStack> itemstacks {};

    std::array<sol::function, 4> luaCallbacks = {};
};
