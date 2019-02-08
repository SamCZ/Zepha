//
// Created by aurailus on 07/02/19.
//

#include <catch.hpp>
#include <enet/enet.h>
#include "../../src/generic/network/Packet.h"

TEST_CASE("Packet <-> ENetPacket", "[networking]") {
    enet_initialize();

    Packet p;

    std::string data("Heya this is a packet");
    Serializer::encodeString(p.data, data);

    ENetPacket* e = p.toENetPacket();

    Packet p2(e);

    for (int i = 0; i < p.data.length(); i++) {
        REQUIRE(p.data[i] == p2.data[i]);
    }

    enet_packet_destroy(e);
    enet_deinitialize();
}