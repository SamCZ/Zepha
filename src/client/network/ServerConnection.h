//
// Created by aurailus on 11/01/19.
//

#ifndef ZEUS_SERVERCONNECTION_H
#define ZEUS_SERVERCONNECTION_H

#include <string>
#include <iostream>
#include <vec3.hpp>

#include "../engine/Timer.h"
#include "../../generic/network/Packet.h"
#include "../../generic/network/NetHandler.h"
#include "../game/Player.h"
#include "../game/entity/PlayerEntity.h"

class ServerConnection {
public:
    ServerConnection(std::string address, unsigned short port);

    void init();
    void update(Player &player, std::vector<PlayerEntity*>& playerEntities);
    void cleanup();

    ~ServerConnection();

    std::vector<Packet> chunkPackets;
    int serverSideChunkGens;
    int recvPackets;
private:
    bool connected = false;

    NetHandler handler;

    std::string address;
    unsigned short port;
};


#endif //ZEUS_SERVERCONNECTION_H
