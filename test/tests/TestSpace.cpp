//
// Created by aurailus on 2019-11-20.
//

#include <catch2/catch.hpp>
#include <iostream>
#include "../../src/util/Space.h"

TEST_CASE("Space", "[math]") {
    SECTION("Region") {
        SECTION("World") {
            SECTION("From Map Block") {
            REQUIRE(Space::Region::world::fromMapBlock({0, 0, 0}) == glm::ivec3 {0, 0, 0});
            REQUIRE(Space::Region::world::fromMapBlock({1, 1, 1}) == glm::ivec3 {0, 0, 0});
            REQUIRE(Space::Region::world::fromMapBlock({3, 1, 1}) == glm::ivec3 {0, 0, 0});
            REQUIRE(Space::Region::world::fromMapBlock({1, 5, 1}) == glm::ivec3 {0, 1, 0});
            REQUIRE(Space::Region::world::fromMapBlock({1, 1, 10}) == glm::ivec3 {0, 0, 2});
            REQUIRE(Space::Region::world::fromMapBlock({0, -1, 0}) == glm::ivec3 {0, -1, 0});
            REQUIRE(Space::Region::world::fromMapBlock({-3, -5, -4}) == glm::ivec3 {-1, -2, -1});
            REQUIRE(Space::Region::world::fromMapBlock({-8, -9, -15}) == glm::ivec3 {-2, -3, -4});
            REQUIRE(Space::Region::world::fromMapBlock({13, 60, -4}) == glm::ivec3 {3, 15, -1});
            }

            SECTION("From Chunk") {
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {0, 0, 0} * Space::MAPBLOCK_SIZE) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {1, 1, 1} * Space::MAPBLOCK_SIZE) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {3, 1, 1} * Space::MAPBLOCK_SIZE) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {1, 5, 1} * Space::MAPBLOCK_SIZE) == glm::ivec3 {0, 1, 0});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {1, 1, 10} * Space::MAPBLOCK_SIZE) == glm::ivec3 {0, 0, 2});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {0, -1, 0} * Space::MAPBLOCK_SIZE) == glm::ivec3 {0, -1, 0});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {-3, -5, -4} * Space::MAPBLOCK_SIZE) == glm::ivec3 {-1, -2, -1});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {-8, -9, -15} * Space::MAPBLOCK_SIZE) == glm::ivec3 {-2, -3, -4});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {13, 60, -4} * Space::MAPBLOCK_SIZE) == glm::ivec3 {3, 15, -1});
            }

            SECTION("From Block") {
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {0, 0, 0}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {4, 4, 4}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {12, 4, 4}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {4, 20, 4}) == glm::ivec3 {0, 1, 0});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {4, 4, 40}) == glm::ivec3 {0, 0, 2});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {0, -4, 0}) == glm::ivec3 {0, -1, 0});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {-12, -20, -16}) == glm::ivec3 {-1, -2, -1});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {-32, -36, -60}) == glm::ivec3 {-2, -3, -4});
                REQUIRE(Space::Region::world::fromChunk(glm::ivec3 {52, 240, -16}) == glm::ivec3 {3, 15, -1});
            }
        }
    }

    SECTION("MapBlock") {
        SECTION("Relative") {
            SECTION("To Region") {
                REQUIRE(Space::MapBlock::relative::toRegion({0, 0, 0}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::MapBlock::relative::toRegion({0, 5, 0}) == glm::ivec3 {0, 1, 0});
                REQUIRE(Space::MapBlock::relative::toRegion({5, 20, 0}) == glm::ivec3 {1, 0, 0});
                REQUIRE(Space::MapBlock::relative::toRegion({40, 20, 30}) == glm::ivec3 {0, 0, 2});
                REQUIRE(Space::MapBlock::relative::toRegion({0, -5, 0}) == glm::ivec3 {0, 3, 0});
                REQUIRE(Space::MapBlock::relative::toRegion({-10, -7, -3}) == glm::ivec3 {2, 1, 1});
                REQUIRE(Space::MapBlock::relative::toRegion({-16, 0, 0}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::MapBlock::relative::toRegion({-17, -18, -19}) == glm::ivec3 {3, 2, 1});
            }
        }

        SECTION("World") {
            SECTION("From Chunk") {
                REQUIRE(Space::MapBlock::world::fromChunk({0, 0, 0}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::MapBlock::world::fromChunk({1, 1, 1}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::MapBlock::world::fromChunk({3, 1, 1}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::MapBlock::world::fromChunk({1, 5, 1}) == glm::ivec3 {0, 1, 0});
                REQUIRE(Space::MapBlock::world::fromChunk({1, 1, 10}) == glm::ivec3 {0, 0, 2});
                REQUIRE(Space::MapBlock::world::fromChunk({0, -1, 0}) == glm::ivec3 {0, -1, 0});
                REQUIRE(Space::MapBlock::world::fromChunk({-3, -5, -4}) == glm::ivec3 {-1, -2, -1});
                REQUIRE(Space::MapBlock::world::fromChunk({-8, -9, -15}) == glm::ivec3 {-2, -3, -4});
                REQUIRE(Space::MapBlock::world::fromChunk({13, 60, -4}) == glm::ivec3 {3, 15, -1});
            }

            SECTION("From Block") {
                REQUIRE(Space::MapBlock::world::fromBlock({0, 0, 0}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::MapBlock::world::fromBlock({16, 16, 16}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::MapBlock::world::fromBlock({48, 16, 16}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::MapBlock::world::fromBlock({16, 80, 16}) == glm::ivec3 {0, 1, 0});
                REQUIRE(Space::MapBlock::world::fromBlock({16, 16, 160}) == glm::ivec3 {0, 0, 2});
                REQUIRE(Space::MapBlock::world::fromBlock({0, -16, 0}) == glm::ivec3 {0, -1, 0});
                REQUIRE(Space::MapBlock::world::fromBlock({-48, -80, -64}) == glm::ivec3 {-1, -2, -1});
                REQUIRE(Space::MapBlock::world::fromBlock({-128, -144, -240}) == glm::ivec3 {-2, -3, -4});
                REQUIRE(Space::MapBlock::world::fromBlock({208, 960, -64}) == glm::ivec3 {3, 15, -1});
            }
        }

//        SECTION("Index") {
//            REQUIRE(Space::MapBlock::index({0, 0, 0}) == 0);
//            REQUIRE(Space::MapBlock::index({0, 5, 0}) == 4);
//            REQUIRE(Space::MapBlock::index({5, 20, 0}) == 1);
//            REQUIRE(Space::MapBlock::index({40, 20, 30}) == 32);
//            REQUIRE(Space::MapBlock::index({0, -5, 0}) == 12);
//            REQUIRE(Space::MapBlock::index({-10, -7, -3}) == 22);
//            REQUIRE(Space::MapBlock::index({-16, 0, 0}) == 0);
//            REQUIRE(Space::MapBlock::index({3, 3, 3}) == 63);
//
//            REQUIRE(Space::MapBlock::index({0, 0, 0}) == Space::Chunk::index({0, 0, 0}));
//            REQUIRE(Space::MapBlock::index({0, 5, 0}) == Space::Chunk::index({0, 1, 0}));
//            REQUIRE(Space::MapBlock::index({5, 20, 0}) == Space::Chunk::index({1, 0, 0}));
//            REQUIRE(Space::MapBlock::index({40, 20, 30}) == Space::Chunk::index({0, 0, 2}));
//            REQUIRE(Space::MapBlock::index({0, -5, 0}) == Space::Chunk::index({0, 3, 0}));
//            REQUIRE(Space::MapBlock::index({-10, -7, -3}) == Space::Chunk::index({2, 1, 1}));
//            REQUIRE(Space::MapBlock::index({-16, 0, 0}) == Space::Chunk::index({0, 0, 0}));
//            REQUIRE(Space::MapBlock::index({-17, -18, -19}) == Space::Chunk::index({3, 2, 1}));
//        }
    }

    SECTION("Chunk") {
        SECTION("Relative") {
            SECTION("To Map Block") {
                REQUIRE(Space::Chunk::relative::toMapBlock({0, 0, 0}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Chunk::relative::toMapBlock({0, 5, 0}) == glm::ivec3 {0, 1, 0});
                REQUIRE(Space::Chunk::relative::toMapBlock({5, 20, 0}) == glm::ivec3 {1, 0, 0});
                REQUIRE(Space::Chunk::relative::toMapBlock({40, 20, 30}) == glm::ivec3 {0, 0, 2});
                REQUIRE(Space::Chunk::relative::toMapBlock({0, -5, 0}) == glm::ivec3 {0, 3, 0});
                REQUIRE(Space::Chunk::relative::toMapBlock({-10, -7, -3}) == glm::ivec3 {2, 1, 1});
                REQUIRE(Space::Chunk::relative::toMapBlock({-16, 0, 0}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Chunk::relative::toMapBlock({-17, -18, -19}) == glm::ivec3 {3, 2, 1});
            }

            SECTION("To Region") {
                REQUIRE(Space::Chunk::relative::toRegion({0, 0, 0}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Chunk::relative::toRegion({0, 5, 0}) == glm::ivec3 {0, 5, 0});
                REQUIRE(Space::Chunk::relative::toRegion({5, 20, 0}) == glm::ivec3 {5, 4, 0});
                REQUIRE(Space::Chunk::relative::toRegion({40, 20, 30}) == glm::ivec3 {8, 4, 14});
                REQUIRE(Space::Chunk::relative::toRegion({0, -5, 0}) == glm::ivec3 {0, 11, 0});
                REQUIRE(Space::Chunk::relative::toRegion({-10, -7, -3}) == glm::ivec3 {6, 9, 13});
                REQUIRE(Space::Chunk::relative::toRegion({-16, 0, 0}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Chunk::relative::toRegion({-17, -18, -19}) == glm::ivec3 {15, 14, 13});
            }
        }

        SECTION("World") {
            SECTION("From Block") {
                REQUIRE(Space::Chunk::world::fromBlock({0, 0, 0}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Chunk::world::fromBlock({5, 17, 8}) == glm::ivec3 {0, 1, 0});
                REQUIRE(Space::Chunk::world::fromBlock({23, 0, 3}) == glm::ivec3 {1, 0, 0});
                REQUIRE(Space::Chunk::world::fromBlock({0, 0, 32}) == glm::ivec3 {0, 0, 2});
                REQUIRE(Space::Chunk::world::fromBlock({-4, 40, 0}) == glm::ivec3 {-1, 2, 0});
                REQUIRE(Space::Chunk::world::fromBlock({-17, -16, -15}) == glm::ivec3 {-2, -1, -1});
                REQUIRE(Space::Chunk::world::fromBlock({-35, -30, -64}) == glm::ivec3 {-3, -2, -4});
                REQUIRE(Space::Chunk::world::fromBlock({36, 64, -12}) == glm::ivec3 {2, 4, -1});
            }
        }

//        SECTION("Index") {
//            REQUIRE(Space::Chunk::index({0, 0, 0}) == 0);
//            REQUIRE(Space::Chunk::index({0, 5, 0}) == 4);
//            REQUIRE(Space::Chunk::index({5, 20, 0}) == 1);
//            REQUIRE(Space::Chunk::index({40, 20, 30}) == 32);
//            REQUIRE(Space::Chunk::index({0, -5, 0}) == 12);
//            REQUIRE(Space::Chunk::index({-10, -7, -3}) == 22);
//            REQUIRE(Space::Chunk::index({-16, 0, 0}) == 0);
//            REQUIRE(Space::Chunk::index({3, 3, 3}) == 63);
//
//            REQUIRE(Space::Chunk::index({0, 0, 0}) == Space::Chunk::index({0, 0, 0}));
//            REQUIRE(Space::Chunk::index({0, 5, 0}) == Space::Chunk::index({0, 1, 0}));
//            REQUIRE(Space::Chunk::index({5, 20, 0}) == Space::Chunk::index({1, 0, 0}));
//            REQUIRE(Space::Chunk::index({40, 20, 30}) == Space::Chunk::index({0, 0, 2}));
//            REQUIRE(Space::Chunk::index({0, -5, 0}) == Space::Chunk::index({0, 3, 0}));
//            REQUIRE(Space::Chunk::index({-10, -7, -3}) == Space::Chunk::index({2, 1, 1}));
//            REQUIRE(Space::Chunk::index({-16, 0, 0}) == Space::Chunk::index({0, 0, 0}));
//            REQUIRE(Space::Chunk::index({-17, -18, -19}) == Space::Chunk::index({3, 2, 1}));
//        }
    }

    SECTION("Block") {
        SECTION("Relative") {
            SECTION("To Chunk") {
                REQUIRE(Space::Block::relative::toChunk({0, 0, 0}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Block::relative::toChunk({0, 5, 0}) == glm::ivec3 {0, 5, 0});
                REQUIRE(Space::Block::relative::toChunk({5, 20, 0}) == glm::ivec3 {5, 4, 0});
                REQUIRE(Space::Block::relative::toChunk({40, 20, 30}) == glm::ivec3 {8, 4, 14});
                REQUIRE(Space::Block::relative::toChunk({0, -5, 0}) == glm::ivec3 {0, 11, 0});
                REQUIRE(Space::Block::relative::toChunk({-10, -7, -3}) == glm::ivec3 {6, 9, 13});
                REQUIRE(Space::Block::relative::toChunk({-16, 0, 0}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Block::relative::toChunk({-17, -18, -19}) == glm::ivec3 {15, 14, 13});
            }

            SECTION("To Map Block") {
                REQUIRE(Space::Block::relative::toMapBlock({0, 0, 0}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Block::relative::toMapBlock({0, 5, 0}) == glm::ivec3 {0, 5, 0});
                REQUIRE(Space::Block::relative::toMapBlock({5, 20, 0}) == glm::ivec3 {5, 20, 0});
                REQUIRE(Space::Block::relative::toMapBlock({40, 20, 30}) == glm::ivec3 {40, 20, 30});
                REQUIRE(Space::Block::relative::toMapBlock({0, -5, 0}) == glm::ivec3 {0, 59, 0});
                REQUIRE(Space::Block::relative::toMapBlock({-10, -7, -3}) == glm::ivec3 {54, 57, 61});
                REQUIRE(Space::Block::relative::toMapBlock({-16, 0, 0}) == glm::ivec3 {48, 0, 0});
                REQUIRE(Space::Block::relative::toMapBlock({-17, -18, -19}) == glm::ivec3 {47, 46, 45});
            }

            SECTION("To Region") {
                REQUIRE(Space::Block::relative::toRegion({0, 0, 0}) == glm::ivec3 {0, 0, 0});
                REQUIRE(Space::Block::relative::toRegion({0, 5, 0}) == glm::ivec3 {0, 5, 0});
                REQUIRE(Space::Block::relative::toRegion({5, 256, 0}) == glm::ivec3 {5, 0, 0});
                REQUIRE(Space::Block::relative::toRegion({40, 20, 30}) == glm::ivec3 {40, 20, 30});
                REQUIRE(Space::Block::relative::toRegion({0, -5, 0}) == glm::ivec3 {0, 251, 0});
                REQUIRE(Space::Block::relative::toRegion({-10, -7, -3}) == glm::ivec3 {246, 249, 253});
                REQUIRE(Space::Block::relative::toRegion({-16, 0, 0}) == glm::ivec3 {240, 0, 0});
                REQUIRE(Space::Block::relative::toRegion({-17, -18, -19}) == glm::ivec3 {239, 238, 237});
            }
        }

//        SECTION("Index") {
//            REQUIRE(Space::Block::index({0, 0, 0}) == 0);
//            REQUIRE(Space::Block::index({0, 5, 0}) == 80);
//            REQUIRE(Space::Block::index({5, 20, 0}) == 69);
//            REQUIRE(Space::Block::index({40, 20, 30}) == 3656);
//            REQUIRE(Space::Block::index({0, -5, 0}) == 176);
//            REQUIRE(Space::Block::index({-10, -7, -3}) == 3478);
//            REQUIRE(Space::Block::index({-16, 0, 0}) == 0);
//            REQUIRE(Space::Block::index({3, 3, 3}) == 819);
//
//            REQUIRE(Space::Block::index({0, 0, 0}) == Space::Block::index({0, 0, 0}));
//            REQUIRE(Space::Block::index({0, 17, 0}) == Space::Block::index({0, 1, 0}));
//            REQUIRE(Space::Block::index({18, 32, 0}) == Space::Block::index({2, 0, 0}));
//            REQUIRE(Space::Block::index({40, 20, 30}) == Space::Block::index({8, 4, 14}));
//            REQUIRE(Space::Block::index({0, -5, 0}) == Space::Block::index({0, 11, 0}));
//            REQUIRE(Space::Block::index({-10, -7, -3}) == Space::Block::index({6, 9, 13}));
//            REQUIRE(Space::Block::index({-16, 0, 0}) == Space::Block::index({0, 0, 0}));
//            REQUIRE(Space::Block::index({-17, -18, -19}) == Space::Block::index({15, 14, 13}));
//        }
    }
}