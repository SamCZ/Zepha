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
    Server();
    explicit Server(unsigned short port);

    void init();
    void update();
    void cleanup();

    ~Server();
private:
    bool alive = true;

    World world {55};
    NetHandler handler;
    ConnectionList connections;

    unsigned short port;
};


#endif //ZEUS_SERVER_H
