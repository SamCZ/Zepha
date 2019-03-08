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

    std::pair<glm::vec3, glm::vec3> getBounds();
    bool isInBounds(glm::vec3 pos);

    ~ServerPlayer();

    ServerPeer* peer;
private:
    void updateBounds();

    glm::vec3 pos = {0, 0, 0};
    glm::vec3 lastPos = {0, 0, 0};

    glm::vec3 minBounds = {0, 0, 0};
    glm::vec3 maxBounds = {0, 0, 0};
};


#endif //ZEUS_SERVERPLAYER_H
