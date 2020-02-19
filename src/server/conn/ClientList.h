//
// Created by aurailus on 07/02/19.
//

#pragma once

#include <vector>
#include "ServerClient.h"
#include "../../lua/parser/ServerLuaParser.h"

class ClientList {
public:
    explicit ClientList(ServerDefs& defs);

    void handleConnect(ENetEvent e);
    void handleDisconnect(ENetEvent e);

    void createPlayer(std::shared_ptr<ServerClient> c);

    std::vector<std::shared_ptr<ServerClient>> clients;
private:
    ServerDefs& defs;
};
