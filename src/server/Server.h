//
// Created by aurailus on 09/01/19.
//

#ifndef ZEUS_SERVER_H
#define ZEUS_SERVER_H

#include <thread>
#include <vector>
#include <iostream>

#include "player/ServerPlayer.h"
#include "../util/net/Packet.h"
#include "../util/Timer.h"
#include "../util/net/NetHandler.h"
#include "conn/ConnectionList.h"
#include "conn/ServerPeer.h"

class Server {
public:
    explicit Server(unsigned short port);

    void update();
    void cleanup();

    ~Server();
private:
    bool alive = true;

    ServerDefs defs;
    ServerWorld world;
    NetHandler handler;
    ConnectionList connections;

    double elapsedSeconds;
    double deltaTime;

    unsigned short port;
};


#endif //ZEUS_SERVER_H
