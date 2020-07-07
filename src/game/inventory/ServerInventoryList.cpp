//
// Created by aurailus on 2019-10-29.
//

#include "ServerInventoryList.h"

#include "../../util/net/Packet.h"
#include "../../util/net/Serializer.h"
#include "../../server/conn/ClientList.h"
#include "../../lua/api/class/LuaItemStack.h"

ServerInventoryList::ServerInventoryList(DefinitionAtlas& defs, ClientList* list,
    const std::string& invName, const std::string& listName, unsigned short size, unsigned short width) :
    InventoryList(defs, invName, listName, size, width),
    clients(list) {}

void ServerInventoryList::manipulated() {
    dirty = true;
}

bool ServerInventoryList::addWatcher(unsigned int cid) {
    auto& client = clients->getClient(cid);
    if (!client) return false;

    for (const auto& i : watchers) if (i == cid) return false;
    watchers.push_back(cid);

    sendTo(client);
}

bool ServerInventoryList::removeWatcher(unsigned int cid) {
    for (auto it = watchers.cbegin(); it != watchers.cend();) {
        if (*it == cid) {
            watchers.erase(it);
            return true;
        }
        it++;
    }
    return false;
}

Packet ServerInventoryList::createPacket() {
    Serializer s{};

    s.append<std::string>(invName)
     .append<std::string>(listName)
     .append<unsigned int>(itemstacks.size())
     .append<unsigned int>(width);

    for (auto& stack : itemstacks) {
        s.append<unsigned short>(stack.count);
        s.append<unsigned int>(stack.id);
    }

    return s.packet(PacketType::INV_DATA, false);
}

void ServerInventoryList::sendTo(std::shared_ptr<ServerClient> client) {
    if (!client) return;
    auto p = createPacket();
    p.sendTo(client->peer, PacketChannel::INVENTORY);
}

void ServerInventoryList::sendAll() {
    auto p = createPacket();

    for (auto it = watchers.cbegin(); it != watchers.cend();) {
        auto& client = clients->getClient(*it);
        if (!client) {
            it = watchers.erase(it);
        }
        else {
            p.sendTo(client->peer, PacketChannel::INVENTORY);
            it++;
        }
    }
}
