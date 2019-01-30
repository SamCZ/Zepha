//
// Created by aurailus on 11/01/19.
//

#ifndef ZEUS_SERVERCONNECTION_H
#define ZEUS_SERVERCONNECTION_H

#include <string>
#include <iostream>
#include <vec3.hpp>
#include <asio.hpp>

#include "../../generic/network/Packet.h"
#include "../engine/Timer.h"

struct ServerConfig {
    glm::vec3 playerPos;
};

class ServerConnection {
public:
    ServerConnection(std::string address, int port);

    ServerConfig* connect();
    void sendPacket(Packet& p, asio::ip::udp::endpoint& e);

    void update();
//    void handleInPackets();
//    void handlePacket(Packet* packet);

    bool hasInPacket();
    Packet* getPacket();

    ~ServerConnection();

private:
    bool connected;
    std::vector<Packet*> inPackets;

    asio::io_context io_context;
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint remote_endpoint;

    std::string address;
    int port;
};


#endif //ZEUS_SERVERCONNECTION_H
