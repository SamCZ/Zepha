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
#include "Address.h"

class NetHandler {
public:
    NetHandler() = default;
    explicit NetHandler(const Address& hostAddress);
    NetHandler(const Address& hostAddress, int connection_attempts, int connection_timeout);

    NetHandler(unsigned short port, short max_clients);

    void disconnect();

    NetState getState();
    ENetPeer* getPeer();
    bool update(ENetEvent* event);

    static std::string intToIPString(unsigned int ip);

    ~NetHandler();

    const static int PACKET_CHANNELS = 12;
private:
    void initServer(unsigned short port, short max_clients);
    void initClient(Address hostAddress, int connection_attempts, int connection_timeout);

    bool initialized = false; //Not default constructed.
    NetState state = NetState::UNINITIALIZED;

    ENetPeer* peer = nullptr;
    ENetHost* host = nullptr;

    ENetAddress address {};
};

#endif //ZEUS_NETHANDLER_H
