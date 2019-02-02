//
// Created by aurailus on 09/01/19.
//

#ifndef ZEUS_SERVER_H
#define ZEUS_SERVER_H

#include <thread>
#include <vector>
#include <iostream>
#include <asio.hpp>

#include "ServerPlayer.h"
#include "network/ConnMan.h"
#include "network/ServerClient.h"
#include "../generic/network/Packet.h"
#include "../client/engine/Timer.h"

using asio::ip::udp;

class Server {
public:
    Server();
    explicit Server(unsigned short port);

    void init();
    void loop();
    void cleanup();

    ~Server();
private:
    ConnMan conn;
    std::map<std::string, ServerPlayer*>* players;

    void sendChunks(ServerPlayer &player);

    unsigned short port;
    bool alive = true;
};


#endif //ZEUS_SERVER_H
