//
// Created by aurailus on 09/01/19.
//

#ifndef ZEUS_SERVER_H
#define ZEUS_SERVER_H

#include <thread>
#include <vector>
#include <iostream>
#include <enet/enet.h>

#include "ServerPlayer.h"
#include "network/ConnMan.h"
#include "network/ServerClient.h"
#include "../generic/network/Packet.h"
#include "../client/engine/Timer.h"

class Server {
public:
    Server();
    explicit Server(unsigned short port);

    void init();
    void update();
    void cleanup();

    ~Server();
private:
    bool alive = true;

    ENetAddress address;
    ENetHost* server;

    unsigned short port;
};


#endif //ZEUS_SERVER_H
