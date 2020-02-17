//
// Created by aurailus on 07/02/19.
//

#pragma once

#include <enet/enet.h>
#include <glm/vec3.hpp>
#include <unordered_map>

#include "../../util/Vec.h"

class ServerClient {
public:
    const static int CHUNK_SEND_RANGE = 32;

    ServerClient(ENetPeer* peer, ENetAddress address);

    void setPos(glm::vec3 pos);
    glm::vec3 getPos();

    void setAngle(float angle);
    float getAngle();

    void setMapBlockIntegrity(glm::ivec3 pos, unsigned long long integrity);
    unsigned long long getMapBlockIntegrity(glm::ivec3 pos);

    bool hasPlayer = false;

    unsigned int cid;
    std::string username;

    ENetPeer* peer;
    ENetAddress address;

    bool changedMapBlocks = true;
    glm::vec3 lastPos {-10000000, -10000000, -10000000};

private:
    glm::vec3 pos {};
    float angle = 0;

    std::unordered_map<glm::ivec3, unsigned long long, Vec::ivec3> mapBlockIntegrity {};
};
