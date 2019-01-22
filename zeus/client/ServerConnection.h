//
// Created by aurailus on 11/01/19.
//

#ifndef ZEUS_SERVERCONNECTION_H
#define ZEUS_SERVERCONNECTION_H

#include <string>
#include <vec3.hpp>
#include <asio.hpp>
#include "../engine/Timer.h"
#include "../server/Packet.h"

struct ServerConfig {
    glm::vec3 playerPos;
};

class ServerConnection {
public:
    ServerConnection(std::string address, int port);

    ServerConfig* connect();
    void sendPacket(Packet& p, asio::ip::udp::endpoint& e);

    void update();
    void handlePacket(Packet &packet, asio::ip::udp::endpoint* endpoint);
    void reqChunks(glm::vec3 a, glm::vec3 b);

    ~ServerConnection();

private:
    bool connected;

    asio::io_context io_context;
    asio::ip::udp::socket socket;
    asio::ip::udp::endpoint remote_endpoint;

    std::string address;
    int port;
};


#endif //ZEUS_SERVERCONNECTION_H
