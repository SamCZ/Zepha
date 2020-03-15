//
// Created by aurailus on 2020-03-07.
//

#pragma once

#include <memory>
#include <enet/enet.h>

#include "PacketType.h"
#include "Deserializer.h"

class PacketView {
public:
    explicit PacketView(ENetPacket* packet);
    PacketView(const PacketView&) = delete;

    std::shared_ptr<ENetPacket> packet;
    Deserializer d;
    PacketType type;
};
