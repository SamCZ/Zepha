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

#include "PacketType.h"
#include "PacketChannel.h"

class Packet {
public:
    Packet() = default;
    explicit Packet(PacketType type, bool reliable = true);
    explicit Packet(ENetPacket* packet);

    ENetPacket* toENetPacket();
    void sendTo(ENetPeer* peer, PacketChannel channel);
    void sendTo(const ENetPeer &peer, PacketChannel channel);

    PacketType type = PacketType::UNDEFINED;
    std::string data {};
    bool reliable = true;
};

