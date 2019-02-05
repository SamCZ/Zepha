//
// Created by aurailus on 11/01/19.
//

#ifndef ZEUS_SERVERCONNECTION_H
#define ZEUS_SERVERCONNECTION_H

#include <string>
#include <iostream>
#include <vec3.hpp>
#include <enet/enet.h>

#include "../../generic/network/Packet.h"
#include "../engine/Timer.h"

class ServerConnection {
public:
    ServerConnection(std::string address, unsigned short port);

    void init();
    void update();
    void cleanup();

    ~ServerConnection();

private:
    bool connected = false;

    ENetHost* client;
    ENetPeer* server;

    int sendInterval = 0;
    int sendCount = 0;

    std::string address;
    unsigned short port;
};


#endif //ZEUS_SERVERCONNECTION_H
