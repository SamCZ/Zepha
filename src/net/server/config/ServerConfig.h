//
// Created by aurailus on 03/07/19.
//

#pragma once

#include "../../PacketView.h"

class PacketView;
class ServerGame;
class ServerClient;

class ServerConfig {
public:
    explicit ServerConfig(ServerGame& defs);

    void init();

    //Bool: Create player
    bool handlePacket(ServerClient &client, PacketView& p);
private:
    ServerGame& game;
    std::vector<std::string> blockIdentifierList {};
    std::vector<std::string> biomeIdentifierList {};
};
