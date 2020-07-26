//
// Created by aurailus on 07/02/19.
//

#pragma once

#include <vector>

#include "ServerClient.h"

class ServerSubgame;

class ClientList {
public:
    explicit ClientList(ServerSubgame& defs);

    void handleConnect(ENetEvent e, InventoryRefs& refs);
    void handleDisconnect(ENetEvent e);

    void createPlayer(std::shared_ptr<ServerClient> c);
    const std::shared_ptr<ServerClient> getClient(unsigned int cid) const;

    std::vector<std::shared_ptr<ServerClient>> clients;
private:
    ServerSubgame& defs;
};
