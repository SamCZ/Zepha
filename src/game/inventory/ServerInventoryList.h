//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include <list>
#include <vector>
#include <functional>

#include "ItemStack.h"

class ClientList;
class ServerClient;
class Packet;

class ServerInventoryList {
public:
    enum class Callback { ALLOW_TAKE, ALLOW_PUT, ON_TAKE, ON_PUT };

    ServerInventoryList(DefinitionAtlas& defs, ClientList* list, const std::string& invName,
                        const std::string& name, unsigned short size, unsigned short width);

    unsigned short getLength();
    unsigned short getWidth();
    std::string getName();

    // Place the stack at i into the existing stack, returning overflow or other stack.
    ItemStack placeStack(unsigned short i, const ItemStack& stack, bool playerInitiated = false);
    // Split the stack at i and return half of it, rounded up
    ItemStack splitStack(unsigned short i, bool playerInitiated = false);
    // Adds as much of `stack` to the inventory as possible, returns leftover.
    ItemStack addStack(ItemStack stack, bool playerInitiated = false);
    // Returns how many of `stack`'s items fit, from 0 to stack.count.
    unsigned short stackFits(const ItemStack& stack);
    // Attempts to remove the usertype and amount of items specified by `stack`. Returns actual ItemStack of what was removed.
    ItemStack takeStack(ItemStack request, bool playerInitiated = false);

    void setStack(unsigned short i, const ItemStack& stack);
    ItemStack getStack(unsigned short i);

    // Removes up to count items from ind, returns the items removed
    ItemStack removeStack(unsigned short ind, unsigned short count);

    bool addWatcher(unsigned int cid);
    bool removeWatcher(unsigned int cid);

    void sendAll();
    void sendTo(std::shared_ptr<ServerClient> client);

    void setLuaCallback(Callback type, sol::function cb);
    sol::function getLuaCallback(Callback type);

    DefinitionAtlas& defs;
    bool dirty = false;
private:
    void setDirty();
    Packet createPacket();

    std::vector<ItemStack> itemstacks {};
    unsigned short width = 0;
    std::string name;
    std::string invName;

    std::array<sol::function, 4> luaCallbacks = {};

    ClientList* clients;
    std::list<unsigned int> watchers;
};
