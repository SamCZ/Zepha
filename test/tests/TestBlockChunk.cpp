//
// Created by aurailus on 2019-12-23.
//

#include <catch2/catch.hpp>
#include <iostream>
#include "../../src/world/chunk/BlockChunk.h"

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedGlobalDeclarationInspection"

TEST_CASE("BlockChunk", "[world]") {
    SECTION("Lighting") {
        BlockChunk b;

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

    SECTION("Blocks") {

        SECTION("Exact index = 0, strip one after") {
            BlockChunk b {{0, 1, 1, 0}, {}};

            b.setBlock(0, 2);

            REQUIRE(b.cGetBlocks().size() == 4);
            REQUIRE(b.cGetBlocks()[0] == 0);
            REQUIRE(b.cGetBlocks()[1] == 2);
            REQUIRE(b.cGetBlocks()[2] == 1);
            REQUIRE(b.cGetBlocks()[3] == 0);
        }

        SECTION("Exact index, last block same, strip one after") {
            BlockChunk b {{0, 1, 1, 0, 2, 5}, {}};

            b.setBlock(1, 1);

            REQUIRE(b.cGetBlocks().size() == 4);
            REQUIRE(b.cGetBlocks()[0] == 0);
            REQUIRE(b.cGetBlocks()[1] == 1);
            REQUIRE(b.cGetBlocks()[2] == 2);
            REQUIRE(b.cGetBlocks()[3] == 5);
        }

        SECTION("Exact index, last block same, strip two after") {
            BlockChunk b {{0, 1, 2, 0, 4, 5}, {}};

            b.setBlock(2, 1);

            REQUIRE(b.cGetBlocks().size() == 6);
            REQUIRE(b.cGetBlocks()[0] == 0);
            REQUIRE(b.cGetBlocks()[1] == 1);
            REQUIRE(b.cGetBlocks()[2] == 3);
            REQUIRE(b.cGetBlocks()[3] == 0);
            REQUIRE(b.cGetBlocks()[4] == 4);
            REQUIRE(b.cGetBlocks()[5] == 5);

            SECTION("Exact index, last block same, strip one after AFTER two after") {
                b.setBlock(3, 1);

                REQUIRE(b.cGetBlocks().size() == 4);
                REQUIRE(b.cGetBlocks()[0] == 0);
                REQUIRE(b.cGetBlocks()[1] == 1);
                REQUIRE(b.cGetBlocks()[4] == 4);
                REQUIRE(b.cGetBlocks()[5] == 5);
            }
        }

        SECTION("Exact index, last block same, no strip after") {
            BlockChunk b {{0, 1, 1, 0}, {}};

            b.setBlock(1, 1);

            REQUIRE(b.cGetBlocks().size() == 4);
            REQUIRE(b.cGetBlocks()[0] == 0);
            REQUIRE(b.cGetBlocks()[1] == 1);
            REQUIRE(b.cGetBlocks()[2] == 2);
            REQUIRE(b.cGetBlocks()[3] == 0);
        }

        SECTION("Exact index, last block different, strip one after") {
            BlockChunk b {{0, 1, 1, 0, 2, 5}, {}};

            b.setBlock(1, 2);

            REQUIRE(b.cGetBlocks().size() == 6);
            REQUIRE(b.cGetBlocks()[0] == 0);
            REQUIRE(b.cGetBlocks()[1] == 1);
            REQUIRE(b.cGetBlocks()[2] == 1);
            REQUIRE(b.cGetBlocks()[3] == 2);
            REQUIRE(b.cGetBlocks()[4] == 2);
            REQUIRE(b.cGetBlocks()[5] == 5);
        }

        SECTION("Exact index, last block different, strip two after") {
            BlockChunk b {{0, 1, 2, 0, 4, 5}, {}};

            b.setBlock(2, 2);

            REQUIRE(b.cGetBlocks().size() == 8);
            REQUIRE(b.cGetBlocks()[0] == 0);
            REQUIRE(b.cGetBlocks()[1] == 1);
            REQUIRE(b.cGetBlocks()[2] == 2);
            REQUIRE(b.cGetBlocks()[3] == 2);
            REQUIRE(b.cGetBlocks()[4] == 3);
            REQUIRE(b.cGetBlocks()[5] == 0);
            REQUIRE(b.cGetBlocks()[6] == 4);
            REQUIRE(b.cGetBlocks()[7] == 5);

            SECTION("Exact index, last block different, strip one after AFTER two after") {
                b.setBlock(3, 1);

                REQUIRE(b.cGetBlocks().size() == 8);
                REQUIRE(b.cGetBlocks()[0] == 0);
                REQUIRE(b.cGetBlocks()[1] == 1);
                REQUIRE(b.cGetBlocks()[2] == 2);
                REQUIRE(b.cGetBlocks()[3] == 2);
                REQUIRE(b.cGetBlocks()[4] == 3);
                REQUIRE(b.cGetBlocks()[5] == 1);
                REQUIRE(b.cGetBlocks()[6] == 4);
                REQUIRE(b.cGetBlocks()[7] == 5);
            }

            SECTION("Exact index, last block same, strip one after AFTER two after") {
                b.setBlock(3, 2);

                REQUIRE(b.cGetBlocks().size() == 6);
                REQUIRE(b.cGetBlocks()[0] == 0);
                REQUIRE(b.cGetBlocks()[1] == 1);
                REQUIRE(b.cGetBlocks()[2] == 2);
                REQUIRE(b.cGetBlocks()[3] == 2);
                REQUIRE(b.cGetBlocks()[4] == 4);
                REQUIRE(b.cGetBlocks()[5] == 5);
            }
        }

        SECTION("Exact index, last block different, no strip after") {
            BlockChunk b {{0, 1, 1, 0}, {}};

            b.setBlock(1, 2);

            REQUIRE(b.cGetBlocks().size() == 6);
            REQUIRE(b.cGetBlocks()[0] == 0);
            REQUIRE(b.cGetBlocks()[1] == 1);
            REQUIRE(b.cGetBlocks()[2] == 1);
            REQUIRE(b.cGetBlocks()[3] == 2);
            REQUIRE(b.cGetBlocks()[4] == 2);
            REQUIRE(b.cGetBlocks()[5] == 0);
        }

        SECTION("Greater index, last block same, strip one after") {
            BlockChunk b {{0, 1, 1, 0, 3, 5}, {}};

            b.setBlock(2, 0);

            // The function should exit early because of the getBlock check
            REQUIRE(b.cGetBlocks().size() == 6);
            REQUIRE(b.cGetBlocks()[0] == 0);
            REQUIRE(b.cGetBlocks()[1] == 1);
            REQUIRE(b.cGetBlocks()[2] == 1);
            REQUIRE(b.cGetBlocks()[3] == 0);
            REQUIRE(b.cGetBlocks()[4] == 3);
            REQUIRE(b.cGetBlocks()[5] == 5);
        }

        SECTION("Greater index, last block same, strip two after") {
            BlockChunk b {{0, 1, 2, 0, 5, 5}, {}};

            b.setBlock(3, 0);

            // The function should exit early because of the getBlock check
            REQUIRE(b.cGetBlocks().size() == 6);
            REQUIRE(b.cGetBlocks()[0] == 0);
            REQUIRE(b.cGetBlocks()[1] == 1);
            REQUIRE(b.cGetBlocks()[2] == 2);
            REQUIRE(b.cGetBlocks()[3] == 0);
            REQUIRE(b.cGetBlocks()[4] == 5);
            REQUIRE(b.cGetBlocks()[5] == 5);
        }

        SECTION("Greater index, last block same, no strip after") {
            BlockChunk b {{0, 1, 3, 0}, {}};

            b.setBlock(1, 1);

            // The function should exit early because of the getBlock check
            REQUIRE(b.cGetBlocks().size() == 4);
            REQUIRE(b.cGetBlocks()[0] == 0);
            REQUIRE(b.cGetBlocks()[1] == 1);
            REQUIRE(b.cGetBlocks()[2] == 3);
            REQUIRE(b.cGetBlocks()[3] == 0);
        }

        SECTION("Greater index, last block different, strip one after") {
            SECTION("Indexed block different") {
                BlockChunk b {{0, 1, 1, 0, 3, 5}, {}};

                b.setBlock(2, 2);

                REQUIRE(b.cGetBlocks().size() == 8);
                REQUIRE(b.cGetBlocks()[0] == 0);
                REQUIRE(b.cGetBlocks()[1] == 1);
                REQUIRE(b.cGetBlocks()[2] == 1);
                REQUIRE(b.cGetBlocks()[3] == 0);
                REQUIRE(b.cGetBlocks()[4] == 2);
                REQUIRE(b.cGetBlocks()[5] == 2);
                REQUIRE(b.cGetBlocks()[6] == 3);
                REQUIRE(b.cGetBlocks()[7] == 5);
            }

            SECTION("Indexed block same") {
                BlockChunk b {{0, 1, 1, 0, 3, 5}, {}};

                b.setBlock(2, 5);

                REQUIRE(b.cGetBlocks().size() == 6);
                REQUIRE(b.cGetBlocks()[0] == 0);
                REQUIRE(b.cGetBlocks()[1] == 1);
                REQUIRE(b.cGetBlocks()[2] == 1);
                REQUIRE(b.cGetBlocks()[3] == 0);
                REQUIRE(b.cGetBlocks()[4] == 2);
                REQUIRE(b.cGetBlocks()[5] == 5);
            }
        }

        SECTION("Greater index, last block different, strip three after") {
            BlockChunk b {{0, 1, 2, 0, 6, 6}, {}};

            b.setBlock(3, 5);

            REQUIRE(b.cGetBlocks().size() == 10);
            REQUIRE(b.cGetBlocks()[0] == 0);
            REQUIRE(b.cGetBlocks()[1] == 1);
            REQUIRE(b.cGetBlocks()[2] == 2);
            REQUIRE(b.cGetBlocks()[3] == 0);
            REQUIRE(b.cGetBlocks()[4] == 3);
            REQUIRE(b.cGetBlocks()[5] == 5);
            REQUIRE(b.cGetBlocks()[6] == 4);
            REQUIRE(b.cGetBlocks()[7] == 0);
            REQUIRE(b.cGetBlocks()[8] == 6);
            REQUIRE(b.cGetBlocks()[9] == 6);

            SECTION("Greater index, last block same, strip one after AFTER three after") {
                b.setBlock(4, 5);

                REQUIRE(b.cGetBlocks().size() == 10);
                REQUIRE(b.cGetBlocks()[0] == 0);
                REQUIRE(b.cGetBlocks()[1] == 1);
                REQUIRE(b.cGetBlocks()[2] == 2);
                REQUIRE(b.cGetBlocks()[3] == 0);
                REQUIRE(b.cGetBlocks()[4] == 3);
                REQUIRE(b.cGetBlocks()[5] == 5);
                REQUIRE(b.cGetBlocks()[6] == 5);
                REQUIRE(b.cGetBlocks()[7] == 0);
                REQUIRE(b.cGetBlocks()[8] == 6);
                REQUIRE(b.cGetBlocks()[9] == 6);
            }

            SECTION("Greater index, last block different, strip one after AFTER three after") {
                b.setBlock(4, 2);

                REQUIRE(b.cGetBlocks().size() == 12);
                REQUIRE(b.cGetBlocks()[0] == 0);
                REQUIRE(b.cGetBlocks()[1] == 1);
                REQUIRE(b.cGetBlocks()[2] == 2);
                REQUIRE(b.cGetBlocks()[3] == 0);
                REQUIRE(b.cGetBlocks()[4] == 3);
                REQUIRE(b.cGetBlocks()[5] == 5);
                REQUIRE(b.cGetBlocks()[6] == 4);
                REQUIRE(b.cGetBlocks()[7] == 2);
                REQUIRE(b.cGetBlocks()[8] == 5);
                REQUIRE(b.cGetBlocks()[9] == 0);
                REQUIRE(b.cGetBlocks()[10] == 6);
                REQUIRE(b.cGetBlocks()[11] == 6);
            }
        }

        SECTION("No index found in loop, last block different, not end of chunk") {
            BlockChunk b {{0, 1, 2, 0}, {}};

            b.setBlock(6, 6);

            REQUIRE(b.cGetBlocks().size() == 8);
            REQUIRE(b.cGetBlocks()[0] == 0);
            REQUIRE(b.cGetBlocks()[1] == 1);
            REQUIRE(b.cGetBlocks()[2] == 2);
            REQUIRE(b.cGetBlocks()[3] == 0);
            REQUIRE(b.cGetBlocks()[4] == 6);
            REQUIRE(b.cGetBlocks()[5] == 6);
            REQUIRE(b.cGetBlocks()[6] == 7);
            REQUIRE(b.cGetBlocks()[7] == 0);
        }

        SECTION("No index found in loop, last block different, end of chunk") {
            BlockChunk b {{0, 1, 2, 0}, {}};

            b.setBlock(4095, 6);

            REQUIRE(b.cGetBlocks().size() == 6);
            REQUIRE(b.cGetBlocks()[0] == 0);
            REQUIRE(b.cGetBlocks()[1] == 1);
            REQUIRE(b.cGetBlocks()[2] == 2);
            REQUIRE(b.cGetBlocks()[3] == 0);
            REQUIRE(b.cGetBlocks()[4] == 4095);
            REQUIRE(b.cGetBlocks()[5] == 6);
        }
    }
}

#pragma clang diagnostic pop