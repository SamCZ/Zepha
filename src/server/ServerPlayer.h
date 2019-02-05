//
// Created by aurailus on 11/01/19.
//

#ifndef ZEUS_SERVERPLAYER_H
#define ZEUS_SERVERPLAYER_H


#include <vec3.hpp>
//#include "network/ServerClient.h"

class ServerPlayer {
public:
    ServerPlayer();
//    ServerPlayer(ServerClient* connection, glm::vec3 pos);

//    ServerClient* connection;
    glm::vec3 pos = glm::vec3(0, 0, 0);

//    glm::vec3 lastSentPos = glm::vec3(0, 0, 0);
//    bool forceSendChunks = false;
};


#endif //ZEUS_SERVERPLAYER_H
