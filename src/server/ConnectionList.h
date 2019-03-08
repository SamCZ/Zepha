//
// Created by aurailus on 07/02/19.
//

#ifndef ZEUS_SERVERPLAYERLIST_H
#define ZEUS_SERVERPLAYERLIST_H

#include <vector>
#include <string>
#include "ServerPlayer.h"
#include "ServerPeer.h"
#include "world/World.h"

class ConnectionList {
public:
    explicit ConnectionList(World* world);

    ServerPeer* addPeer(ENetPeer* peer);
    void removePeer(ENetPeer* peer);

    ServerPlayer* createPlayer(ServerPeer *peer, std::string uuid);
public:
    std::vector<ServerPeer*> peers;
    World* world;
};


#endif //ZEUS_SERVERPLAYERLIST_H
