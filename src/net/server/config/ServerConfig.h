//
// Created by aurailus on 03/07/19.
//

#pragma once

#include "../../PacketView.h"

class PacketView;
class ServerSubgame;
class ServerClient;

class ServerConfig {
public:
    explicit ServerConfig(ServerSubgame& defs);

    void init();

    //Bool: Create player
    bool handlePacket(ServerClient &client, PacketView& p);
private:
    ServerSubgame& game;
    std::vector<std::string> blockIdentifierList {};
    std::vector<std::string> biomeIdentifierList {};
};
