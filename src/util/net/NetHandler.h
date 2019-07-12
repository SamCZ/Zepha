//
// Created by aurailus on 03/02/19.
//

#ifndef ZEUS_NETHANDLER_H
#define ZEUS_NETHANDLER_H

#include <string>
#include <iostream>
#include <enet/enet.h>
#include "PacketChannel.h"
#include "../Log.h"
#include "NetState.h"

class NetHandler {
public:
    NetHandler();
    NetHandler(unsigned short port, short max_clients);
    NetHandler(std::string host_address, unsigned short host_port);
    NetHandler(std::string host_address, unsigned short host_port, int connection_attempts, int connection_timeout);

    void disconnect();

    NetState getState();
    ENetPeer* getPeer();
    bool update(ENetEvent* event);

    ~NetHandler();
private:
    void initServer(unsigned short port, short max_clients);
    void initClient(std::string host_address, unsigned short host_port, int connection_attempts, int connection_timeout);

    bool initialized = false; //Not default constructed.
    NetState state = NetState::UNINITIALIZED;

    ENetPeer* peer;
    ENetHost* host;

    ENetAddress address;

    const static int PACKET_CHANNELS = 12;
};

#endif //ZEUS_NETHANDLER_H
