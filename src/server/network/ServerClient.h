//
// Created by aurailus on 10/01/19.
//

#ifndef ZEUS_CLIENTCONNECTION_H
#define ZEUS_CLIENTCONNECTION_H

#include <asio.hpp>

class ServerPlayer;

class ServerClient {
public:
    ServerClient();
    ServerClient(asio::ip::udp::endpoint* endpoint, std::string uuid);

    ServerPlayer* player;

    std::string uuid;
    bool authenticated;

    asio::ip::udp::endpoint* endpoint;

    ~ServerClient();
};


#endif //ZEUS_CLIENTCONNECTION_H
