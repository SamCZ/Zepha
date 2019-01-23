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
#include "network/ClientConnection.h"
#include "../generic/network/Packet.h"
#include "../client/engine/Timer.h"

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

    std::string createIdentifier(udp::endpoint* endpoint);

    void handlePacket(Packet& packet, udp::endpoint* endpoint);
    void handleAuthPacket(std::string& identifier, Packet& packet, udp::endpoint* endpoint);

    void addConnection(std::string& identifier, udp::endpoint* endpoint);
    void createPlayer(ClientConnection* connection);

    //string is IP:Port
    std::map<std::string, ClientConnection*> connections;
    //string is username
    std::map<std::string, ServerPlayer*> players;

    int port;
    bool alive;
    asio::io_context io_context;
    udp::socket* server_socket;
};


#endif //ZEUS_SERVER_H
