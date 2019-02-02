//
// Created by aurailus on 22/01/19.
//

#ifndef ZEUS_CONNMAN_H
#define ZEUS_CONNMAN_H

#include <asio.hpp>
#include "ServerClient.h"
#include "../ServerPlayer.h"
#include "../../generic/network/Packet.h"

using asio::ip::udp;

class ConnMan {
public:
    ConnMan();

    void init(unsigned short port);
    void poll();
    void send(Packet* p, ServerClient* connection);
    void send(Packet* p, udp::endpoint* endpoint);

    std::map<std::string, ServerPlayer*>* getPlayersMap();

    ~ConnMan();
private:
    void handlePacket(Packet* packet, udp::endpoint* endpoint);
    std::string createIdentifier(udp::endpoint *endpoint);

    ServerClient* addClient(std::string uuid, udp::endpoint* endpoint);
    void addPlayer(ServerClient* client, std::string username);

    std::map<std::string, ServerClient*> clients;
    std::map<std::string, ServerPlayer*> players;


//    std::vector<std::pair<udp::endpoint*, Packet>> packets;

    asio::io_context io_context;
    udp::socket* server_socket;
};


#endif //ZEUS_CONNMAN_H
