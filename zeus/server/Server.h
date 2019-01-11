//
// Created by aurailus on 09/01/19.
//

#ifndef ZEUS_SERVER_H
#define ZEUS_SERVER_H

#include <thread>
#include <vector>
#include "../engine/Timer.h"
#include "ClientConnection.h"
#include "Packet.h"
#include <iostream>
#include <asio.hpp>

using asio::ip::udp;

class Server {
public:
    Server();
    explicit Server(int port);

    void start();

    ~Server();
private:
    void loop();
    void cleanup();

    void handlePacket(Packet& packet, udp::endpoint* endpoint);

    std::map<std::string, ClientConnection*> connections;

    int port;
    bool alive;
    asio::io_context io_context;
    udp::socket* server_socket;
};


#endif //ZEUS_SERVER_H
