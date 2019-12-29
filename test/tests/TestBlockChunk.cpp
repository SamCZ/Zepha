//
// Created by aurailus on 2019-12-23.
//

#include <catch2/catch.hpp>
#include <iostream>
#include "../../src/world/chunk/BlockChunk.h"

TEST_CASE("BlockChunk", "[world]") {
    BlockChunk b;
    
    SECTION("Lighting") {
        b.setSunlight(1, 4);
        b.setSunlight(2, 1);
        b.setSunlight(3, 11);
        b.setSunlight(4, 5);
        b.setSunlight(100, 15);
        b.setSunlight(3000, 0);

        b.setBlocklight(1, 4);
        b.setBlocklight(2, 1);
        b.setBlocklight(3, 11);
        b.setBlocklight(4, 5);
        b.setBlocklight(100, 15);
        b.setBlocklight(3000, 0);

        SECTION("Sunlight") {
            REQUIRE(b.getSunlight(1) == 4);
            REQUIRE(b.getSunlight(2) == 1);
            REQUIRE(b.getSunlight(3) == 11);
            REQUIRE(b.getSunlight(4) == 5);
            REQUIRE(b.getSunlight(100) == 15);
            REQUIRE(b.getSunlight(3000) == 0);
        }

        SECTION("Blocklight") {
            REQUIRE(b.getBlocklight(1) == 4);
            REQUIRE(b.getBlocklight(2) == 1);
            REQUIRE(b.getBlocklight(3) == 11);
            REQUIRE(b.getBlocklight(4) == 5);
            REQUIRE(b.getBlocklight(100) == 15);
            REQUIRE(b.getBlocklight(3000) == 0);
        }
    }
}