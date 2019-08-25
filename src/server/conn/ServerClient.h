//
// Created by aurailus on 07/02/19.
//

#pragma once

#include <enet/enet.h>
#include "ServerPlayer.h"

class ServerClient {
public:
    ServerClient(ENetPeer* peer, ENetAddress address);

    void initPlayer();

    bool hasPlayer();
    ServerPlayer& getPlayer();
    ENetPeer& getPeer();

    unsigned int getConnectID();

    ~ServerClient();
private:
    ENetPeer* peer;
    ENetAddress address;

    ServerPlayer* player = nullptr;

    unsigned int connectID;
};
