//
// Packet implentation for easy manipulation. Can be converted into an ENet packet, or deserialized from one.
//
// Created by aurailus on 10/01/19.
//

#ifndef ZEUS_PACKET_H
#define ZEUS_PACKET_H

#include <vector>
#include <string>
#include <iostream>
#include <enet/enet.h>

#include "Serializer.h"

struct Packet {
    enum {
        UNDEFINED = 0,
        HANDSHAKE,
        AUTHENTICATE,
        PLAYER_INFO,
        ENTITY_INFO,
        CHUNK_INFO
    };

    Packet() = default;
    explicit Packet(int p);
    explicit Packet(ENetPacket* packet);

    ENetPacket* toENetPacket();
    void sendTo(ENetPeer* peer, int channel);

    int type = UNDEFINED;
    std::string data;

    ~Packet() = default;
};


#endif //ZEUS_PACKET_H
