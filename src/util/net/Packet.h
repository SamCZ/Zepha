//
// Packet implentation for easy manipulation. Can be converted into an ENet packet, or deserialized from one.
//
// Created by aurailus on 10/01/19.
//

#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <enet/enet.h>

#include "OLDSerializer.h"
#include "Serializer.h"
#include "PacketType.h"
#include "PacketChannel.h"

class Packet {
public:
    Packet() = default;
    explicit Packet(PacketType type);
    explicit Packet(ENetPacket* packet);

    ENetPacket* toENetPacket();
    void sendTo(ENetPeer* peer, PacketChannel channel);
    void sendTo(const ENetPeer &peer, PacketChannel channel);

    ~Packet() = default;

    PacketType type = PacketType::UNDEFINED;
    std::string data;
};

