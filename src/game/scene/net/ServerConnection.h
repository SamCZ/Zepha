//
// Created by aurailus on 21/07/19.
//

#ifndef ZEUS_SERVERCONNECTION_H
#define ZEUS_SERVERCONNECTION_H


#include <enet/enet.h>
#include <chrono>
#include "../../../util/net/Address.h"

class ServerConnection {
public:
    enum class State {
        UNCONNECTED,
        ATTEMPTING_CONNECT,
        CONNECTED,
        FAILED_CONNECT,
        DISCONNECTED,
        ENET_ERROR
    };

    ServerConnection() = default;

    void attemptConnect(Address addr);
    State getConnectionStatus();
    void disconnect();

    void processConnecting();
    bool pollEvents(ENetEvent* event);

    ENetPeer* getPeer();

    ~ServerConnection();

private:
    unsigned long timeout = 1000;
    unsigned int attempts = 3;

    ENetHost* host = nullptr;
    ENetPeer* peer = nullptr;

    State state = State::UNCONNECTED;
    unsigned int attempt = 0;
    std::chrono::time_point<std::chrono::high_resolution_clock> connectionTime;
};


#endif //ZEUS_SERVERCONNECTION_H
