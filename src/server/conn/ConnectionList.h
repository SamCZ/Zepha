//
// Created by aurailus on 07/02/19.
//

#ifndef ZEUS_SERVERPLAYERLIST_H
#define ZEUS_SERVERPLAYERLIST_H

#include <vector>
#include <string>
#include "../player/ServerPlayer.h"
#include "ServerPeer.h"
#include "../world/ServerWorld.h"

class ConnectionList {
public:
    explicit ConnectionList(ServerWorld* world);

    ServerPeer* addPeer(ENetPeer* peer);
    void removePeer(ENetPeer* peer);

    ServerPlayer* createPlayer(ServerPeer *peer, std::string uuid, std::string username);
public:
    std::vector<ServerPeer*> peers;
    ServerWorld* world;
};


#endif //ZEUS_SERVERPLAYERLIST_H
