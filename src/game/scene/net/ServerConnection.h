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
#include "../../entity/world/PlayerEntity.h"
#include "../../graph/drawable/DrawableGroup.h"
#include "../world/Player.h"
#include "../world/LocalWorld.h"

class ServerConnection {
public:
    ServerConnection(std::string address, unsigned short port, LocalDefs& defs);

    void init(std::vector<Drawable*> &entities, LocalWorld* world);
    void update(Player &player);
    void cleanup();

    void setBlock(glm::vec3 pos, int block);

    ~ServerConnection();

    std::vector<Packet> chunkPackets;
    int serverSideChunkGens = 0;
    int recvPackets = 0;
private:
    std::shared_ptr<AtlasRef> playerFrontTex, playerBackTex, shadowTex;
    bool connected = false;
    int id = 0;

    DrawableGroup* entities;
    LocalWorld* world;

    NetHandler handler;

    std::string address;
    unsigned short port;
};


#endif //ZEUS_SERVERCONNECTION_H
