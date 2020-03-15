//
// Created by aurailus on 03/07/19.
//

#pragma once

#include "../../def/ServerGame.h"

class ServerClient;

class ServerConfig {
public:
    explicit ServerConfig(ServerGame& defs);

    void init();

    //Bool: Create player
    bool handlePacket(ServerClient &client, PacketView& p);
private:
    ServerGame& defs;
    std::vector<std::string> blockIdentifierList {};
    std::vector<std::string> biomeIdentifierList {};
};
