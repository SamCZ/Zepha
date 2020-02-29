//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include <list>

#include "InventoryList.h"

class ClientList;
class ServerClient;
class Packet;

class ServerInventoryList : public InventoryList {
public:
    ServerInventoryList(DefinitionAtlas& defs, ClientList* list, const std::string& invName,
                        const std::string& listName, unsigned short size, unsigned short width);

    bool addWatcher(unsigned int cid);
    bool removeWatcher(unsigned int cid);

    void sendAll();
    void sendTo(std::shared_ptr<ServerClient> client);

    bool dirty = false;

private:
    void manipulated() override;

    Packet createPacket();

    ClientList* clients;
    std::list<unsigned int> watchers;
};
