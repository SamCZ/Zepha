//
// Created by aurailus on 07/02/19.
//

#ifndef ZEUS_SERVERPLAYERLIST_H
#define ZEUS_SERVERPLAYERLIST_H

#include <vector>
#include <string>
#include "ServerPlayer.h"
#include "ServerPeer.h"

class ConnectionList {
public:
    ServerPeer* addPeer(ENetPeer* peer);
    void removePeer(ENetPeer* peer);

    ServerPlayer* addPlayer(ServerPeer* peer, std::string uuid);
public:
    std::vector<ServerPeer*> peers;
    std::vector<ServerPlayer*> players;
};


#endif //ZEUS_SERVERPLAYERLIST_H
