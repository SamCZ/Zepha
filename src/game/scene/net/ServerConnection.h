//
// Created by aurailus on 11/01/19.
//

#ifndef ZEUS_SERVERCONNECTION_H
#define ZEUS_SERVERCONNECTION_H

#include <string>
#include <iostream>
#include <vec3.hpp>

#include "../../../util/Timer.h"
#include "../../../util/net/Packet.h"
#include "../../../util/net/NetHandler.h"
#include "../world/Player.h"
#include "../../entity/world/PlayerEntity.h"
#include "../../graph/drawable/DrawableGroup.h"

class ServerConnection {
public:
    ServerConnection(std::string address, unsigned short port);

    void init(std::vector<Drawable*> &entities);
    void update(Player &player);
    void cleanup();

    ~ServerConnection();

    std::vector<Packet*> chunkPackets;
    int serverSideChunkGens = 0;
    int recvPackets = 0;
private:
    bool connected = false;

    DrawableGroup* entities;

    NetHandler handler;

    std::string address;
    unsigned short port;
};


#endif //ZEUS_SERVERCONNECTION_H
