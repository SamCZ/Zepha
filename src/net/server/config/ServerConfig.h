//
// Created by aurailus on 03/07/19.
//

#pragma once

#include "../../PacketView.h"
#include "../../../util/CovariantPtr.h"

class PacketView;
class ServerClient;
class ServerSubgame;

class ServerConfig {
public:
    explicit ServerConfig(SubgamePtr game);

    void init();

    //Bool: Create player
    bool handlePacket(ServerClient& client, PacketView& p);
private:
    SubgamePtr game;
    std::vector<std::string> blockIdentifierList {};
    std::vector<std::string> biomeIdentifierList {};
};
