//
// Created by aurailus on 2020-06-21.
//

#include <catch2/catch.hpp>
#include "../../src/world/chunk/Chunk.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

TEST_CASE("Chunk Serialization", "[net]") {
    Chunk a = Chunk();
    a.setBlock(5, 20);
    a.setBlock(6, 0);
    a.setBlock(8, 3);
    a.setBlock(9, 3);

    Packet packet(PacketType::CHUNK);
    packet.data = a.serialize();

    ENetPacket* enet = packet.toENetPacket();
    PacketView view(enet);

    Chunk b = Chunk();
    std::string s(view.d.data + 4, view.d.data + view.d.len);
    b.deserialize(s);

    auto itA = a.cGetBlocks();
    auto itB = b.cGetBlocks();

    for (unsigned int i = 0; i < itA.size(); i++) REQUIRE(itA[i] == itB[i]);
}

#pragma clang diagnostic pop
