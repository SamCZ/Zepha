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
    explicit ServerPlayer(ServerPeer* peer);

    Packet getInitPacket();

    ServerPeer* peer;
    glm::vec3 pos = glm::vec3(0, 0, 0);

    ~ServerPlayer();
};


#endif //ZEUS_SERVERPLAYER_H
