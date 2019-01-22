//
// Created by aurailus on 11/01/19.
//

#ifndef ZEUS_SERVERPLAYER_H
#define ZEUS_SERVERPLAYER_H


#include <vec3.hpp>
#include "ClientConnection.h"

class ServerPlayer {
public:
    ServerPlayer() = default;
    ServerPlayer(ClientConnection* connection, glm::vec3 pos);

    ClientConnection* connection;
    glm::vec3 pos;

    std::vector<glm::vec3> requestedChunks;
};


#endif //ZEUS_SERVERPLAYER_H
