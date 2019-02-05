//
// Created by aurailus on 09/01/19.
//

#ifndef ZEUS_SERVER_H
#define ZEUS_SERVER_H

#include <thread>
#include <vector>
#include <iostream>

#include "ServerPlayer.h"
#include "../generic/network/Packet.h"
#include "../client/engine/Timer.h"
#include "../generic/network/NetHandler.h"

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

    NetHandler handler;

    unsigned short port;
};


#endif //ZEUS_SERVER_H
