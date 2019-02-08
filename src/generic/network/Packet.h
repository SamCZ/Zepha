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
    //Type Definitions
    typedef int p_type;

    //Packet Types
    const static p_type UNDEFINED    = 0;
    const static p_type HANDSHAKE    = 1;
    const static p_type AUTHENTICATE = 2;
    const static p_type PLAYERINFO   = 3;
    const static p_type CHUNKINFO    = 4;

    Packet();
    explicit Packet(p_type p);
    explicit Packet(ENetPacket* packet);

    ENetPacket* toENetPacket();

    p_type type = UNDEFINED;
    std::string data;

    ~Packet() = default;
};


#endif //ZEUS_PACKET_H
