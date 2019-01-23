//
// Created by aurailus on 10/01/19.
//

#ifndef ZEUS_CLIENTCONNECTION_H
#define ZEUS_CLIENTCONNECTION_H

#include <asio.hpp>

class ClientConnection {
public:
    ClientConnection();
    explicit ClientConnection(asio::ip::udp::endpoint* endpoint);

    asio::ip::udp::endpoint* endpoint;
    bool authenticated;
    long lastAliveTime;

    ~ClientConnection();
};


#endif //ZEUS_CLIENTCONNECTION_H
