//
// Created by aurailus on 01/02/19.
// Test various functions of the BlockChunk class
//

#include <catch.hpp>
#include "../../src/world/chunk/BlockChunk.h"

BlockChunk* getRandomChunk() {
    auto v = std::vector<int>(4096);
    for (int i = 0; i < 4096; i++) {
        v[i] = (int)random()%10;
    }
    return new BlockChunk(v);
}

TEST_CASE("Blockchunks", "[networking]") {

    SECTION("Serialization") {

        for (int i = 0; i < 20; i++) {
            auto b = getRandomChunk();

//            SECTION("BlockChunk RLE Encoding") {
//                auto rle = b->rleEncode();
//
//                auto b2 = new BlockChunk();
//                b2->rleDecode(rle);
//
//                for (int j = 0; j < 4096; j++) {
//                    REQUIRE(b2->getBlock(j) == b->fromIndex(j));
//                }
//
//                delete b2;
//            }

            auto gzip = b->serialize();

            SECTION("BlockChunk GZip Encoding") {
                auto b2 = new BlockChunk();
                b2->deserialize(gzip);

                for (int j = 0; j < 4096; j++) {
                    REQUIRE(b2->getBlock(j) == b->getBlock(j));
                }

                delete b2;
            }

            SECTION("BlockChunk Packet Encoding") {
                auto p = Packet(Packet::CHUNK_INFO);
                Serializer::encodeString(p.data, gzip);

                auto enetP = p.toENetPacket();

                auto p2 = Packet(enetP);

                int len = Serializer::decodeInt(&p2.data[0]);
                std::string data(p.data.begin() + 4, p.data.begin() + 4 + len);

                auto b2 = new BlockChunk();
                REQUIRE(b2->deserialize(data));

                for (int j = 0; j < 4096; j++) {
                    REQUIRE(b2->getBlock(j) == b->getBlock(j));
                }

                delete b2;
            }

            delete b;

            INFO("Iteration " << i << " passed.");
        }
    }
}