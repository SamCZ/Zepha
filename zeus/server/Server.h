//
// Created by aurailus on 09/01/19.
//

#ifndef ZEUS_SERVER_H
#define ZEUS_SERVER_H

#include <thread>
#include <vector>
#include "../engine/Timer.h"
#include "ClientConnection.h"
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

    static void discovery(bool* alive, std::vector<ClientConnection *>* connections, std::mutex* lock);
    std::vector<ClientConnection*> connections;
    std::mutex connectionsLock;

    int port;
    bool alive;
    asio::io_context io_context;
    udp::socket* server_socket;
};


#endif //ZEUS_SERVER_H
