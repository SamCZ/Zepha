//
// Created by aurailus on 11/01/19.
//

#ifndef ZEUS_SERVERPLAYER_H
#define ZEUS_SERVERPLAYER_H


#include <vec3.hpp>
#include "ServerPeer.h"
#include "../generic/network/Packet.h"

class ServerPlayer {
public:
    const static int ACTIVE_RANGE = 20;

    explicit ServerPlayer(ServerPeer* peer);

    Packet getInitPacket();

    glm::vec3 getPos();
    void setPos(glm::vec3 pos);

    glm::vec3 getChunkPos();

    bool changedChunks = false;

    std::pair<glm::vec3, glm::vec3> getBounds();
    std::pair<glm::vec3, glm::vec3> getOldBounds();

    bool isInBounds(glm::vec3 pos, std::pair<glm::vec3, glm::vec3>& bounds);

    ~ServerPlayer();

    ServerPeer* peer;
private:
    glm::vec3 chunkPos {0, 0, 0};
    glm::vec3 lastChunkPos {0, 0, 0};
    glm::vec3 pos {0, 0, 0};
};


#endif //ZEUS_SERVERPLAYER_H
