//
// Created by aurailus on 07/02/19.
//

#ifndef ZEUS_SERVERPEER_H
#define ZEUS_SERVERPEER_H


#include <enet/enet.h>

class ServerPlayer;

struct ServerPeer {
    ENetPeer* peer;
    ServerPlayer* player;

    void cleanup() {};
};

#endif //ZEUS_SERVERPEER_H
