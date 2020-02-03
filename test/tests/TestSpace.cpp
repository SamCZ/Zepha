//
// Created by aurailus on 2019-11-20.
//

#include <catch2/catch.hpp>
#include <iostream>
#include "../../src/util/Space.h"

using namespace Space;
using namespace glm;

TEST_CASE("Space", "[math]") {
    SECTION("Region") {
        SECTION("World") {
            SECTION("From Map Block") {
            REQUIRE(Region::world::fromMapBlock({0, 0, 0}) == ivec3 {0, 0, 0});
            REQUIRE(Region::world::fromMapBlock({1, 1, 1}) == ivec3 {0, 0, 0});
            REQUIRE(Region::world::fromMapBlock({3, 1, 1}) == ivec3 {0, 0, 0});
            REQUIRE(Region::world::fromMapBlock({1, 5, 1}) == ivec3 {0, 1, 0});
            REQUIRE(Region::world::fromMapBlock({1, 1, 10}) == ivec3 {0, 0, 2});
            REQUIRE(Region::world::fromMapBlock({0, -1, 0}) == ivec3 {0, -1, 0});
            REQUIRE(Region::world::fromMapBlock({-3, -5, -4}) == ivec3 {-1, -2, -1});
            REQUIRE(Region::world::fromMapBlock({-8, -9, -15}) == ivec3 {-2, -3, -4});
            REQUIRE(Region::world::fromMapBlock({13, 60, -4}) == ivec3 {3, 15, -1});
            }

            SECTION("From Chunk") {
                REQUIRE(Region::world::fromChunk(vec3 {0, 0, 0} * static_cast<float>(MAPBLOCK_SIZE)) == ivec3 {0, 0, 0});
                REQUIRE(Region::world::fromChunk(vec3 {1, 1, 1} * static_cast<float>(MAPBLOCK_SIZE)) == ivec3 {0, 0, 0});
                REQUIRE(Region::world::fromChunk(vec3 {3, 1, 1} * static_cast<float>(MAPBLOCK_SIZE)) == ivec3 {0, 0, 0});
                REQUIRE(Region::world::fromChunk(vec3 {1, 5, 1} * static_cast<float>(MAPBLOCK_SIZE)) == ivec3 {0, 1, 0});
                REQUIRE(Region::world::fromChunk(vec3 {1, 1, 10} * static_cast<float>(MAPBLOCK_SIZE)) == ivec3 {0, 0, 2});
                REQUIRE(Region::world::fromChunk(vec3 {0, -1, 0} * static_cast<float>(MAPBLOCK_SIZE)) == ivec3 {0, -1, 0});
                REQUIRE(Region::world::fromChunk(vec3 {-3, -5, -4} * static_cast<float>(MAPBLOCK_SIZE)) == ivec3 {-1, -2, -1});
                REQUIRE(Region::world::fromChunk(vec3 {-8, -9, -15} * static_cast<float>(MAPBLOCK_SIZE)) == ivec3 {-2, -3, -4});
                REQUIRE(Region::world::fromChunk(vec3 {13, 60, -4} * static_cast<float>(MAPBLOCK_SIZE)) == ivec3 {3, 15, -1});
            }

            SECTION("From Block") {
                REQUIRE(Region::world::fromChunk(vec3 {0, 0, 0}) == ivec3 {0, 0, 0});
                REQUIRE(Region::world::fromChunk(vec3 {4, 4, 4}) == ivec3 {0, 0, 0});
                REQUIRE(Region::world::fromChunk(vec3 {12, 4, 4}) == ivec3 {0, 0, 0});
                REQUIRE(Region::world::fromChunk(vec3 {4, 20, 4}) == ivec3 {0, 1, 0});
                REQUIRE(Region::world::fromChunk(vec3 {4, 4, 40}) == ivec3 {0, 0, 2});
                REQUIRE(Region::world::fromChunk(vec3 {0, -4, 0}) == ivec3 {0, -1, 0});
                REQUIRE(Region::world::fromChunk(vec3 {-12, -20, -16}) == ivec3 {-1, -2, -1});
                REQUIRE(Region::world::fromChunk(vec3 {-32, -36, -60}) == ivec3 {-2, -3, -4});
                REQUIRE(Region::world::fromChunk(vec3 {52, 240, -16}) == ivec3 {3, 15, -1});
            }
        }
    }

    SECTION("MapBlock") {
        SECTION("Relative") {
            SECTION("To Region") {
                REQUIRE(MapBlock::relative::toRegion({0, 0, 0}) == ivec3 {0, 0, 0});
                REQUIRE(MapBlock::relative::toRegion({0, 5, 0}) == ivec3 {0, 1, 0});
                REQUIRE(MapBlock::relative::toRegion({5, 20, 0}) == ivec3 {1, 0, 0});
                REQUIRE(MapBlock::relative::toRegion({40, 20, 30}) == ivec3 {0, 0, 2});
                REQUIRE(MapBlock::relative::toRegion({0, -5, 0}) == ivec3 {0, 3, 0});
                REQUIRE(MapBlock::relative::toRegion({-10, -7, -3}) == ivec3 {2, 1, 1});
                REQUIRE(MapBlock::relative::toRegion({-16, 0, 0}) == ivec3 {0, 0, 0});
                REQUIRE(MapBlock::relative::toRegion({-17, -18, -19}) == ivec3 {3, 2, 1});
            }
        }

        SECTION("World") {
            SECTION("From Chunk") {
                REQUIRE(MapBlock::world::fromChunk({0, 0, 0}) == ivec3 {0, 0, 0});
                REQUIRE(MapBlock::world::fromChunk({1, 1, 1}) == ivec3 {0, 0, 0});
                REQUIRE(MapBlock::world::fromChunk({3, 1, 1}) == ivec3 {0, 0, 0});
                REQUIRE(MapBlock::world::fromChunk({1, 5, 1}) == ivec3 {0, 1, 0});
                REQUIRE(MapBlock::world::fromChunk({1, 1, 10}) == ivec3 {0, 0, 2});
                REQUIRE(MapBlock::world::fromChunk({0, -1, 0}) == ivec3 {0, -1, 0});
                REQUIRE(MapBlock::world::fromChunk({-3, -5, -4}) == ivec3 {-1, -2, -1});
                REQUIRE(MapBlock::world::fromChunk({-8, -9, -15}) == ivec3 {-2, -3, -4});
                REQUIRE(MapBlock::world::fromChunk({13, 60, -4}) == ivec3 {3, 15, -1});
            }

            SECTION("From Block") {
                REQUIRE(MapBlock::world::fromBlock({0, 0, 0}) == ivec3 {0, 0, 0});
                REQUIRE(MapBlock::world::fromBlock({16, 16, 16}) == ivec3 {0, 0, 0});
                REQUIRE(MapBlock::world::fromBlock({48, 16, 16}) == ivec3 {0, 0, 0});
                REQUIRE(MapBlock::world::fromBlock({16, 80, 16}) == ivec3 {0, 1, 0});
                REQUIRE(MapBlock::world::fromBlock({16, 16, 160}) == ivec3 {0, 0, 2});
                REQUIRE(MapBlock::world::fromBlock({0, -16, 0}) == ivec3 {0, -1, 0});
                REQUIRE(MapBlock::world::fromBlock({-48, -80, -64}) == ivec3 {-1, -2, -1});
                REQUIRE(MapBlock::world::fromBlock({-128, -144, -240}) == ivec3 {-2, -3, -4});
                REQUIRE(MapBlock::world::fromBlock({208, 960, -64}) == ivec3 {3, 15, -1});
            }
        }

        SECTION("Index") {
            REQUIRE(MapBlock::index({0, 0, 0}) == 0);
            REQUIRE(MapBlock::index({0, 5, 0}) == 4);
            REQUIRE(MapBlock::index({5, 20, 0}) == 1);
            REQUIRE(MapBlock::index({40, 20, 30}) == 32);
            REQUIRE(MapBlock::index({0, -5, 0}) == 12);
            REQUIRE(MapBlock::index({-10, -7, -3}) == 22);
            REQUIRE(MapBlock::index({-16, 0, 0}) == 0);
            REQUIRE(MapBlock::index({3, 3, 3}) == 63);

            REQUIRE(MapBlock::index({0, 0, 0}) == Chunk::index({0, 0, 0}));
            REQUIRE(MapBlock::index({0, 5, 0}) == Chunk::index({0, 1, 0}));
            REQUIRE(MapBlock::index({5, 20, 0}) == Chunk::index({1, 0, 0}));
            REQUIRE(MapBlock::index({40, 20, 30}) == Chunk::index({0, 0, 2}));
            REQUIRE(MapBlock::index({0, -5, 0}) == Chunk::index({0, 3, 0}));
            REQUIRE(MapBlock::index({-10, -7, -3}) == Chunk::index({2, 1, 1}));
            REQUIRE(MapBlock::index({-16, 0, 0}) == Chunk::index({0, 0, 0}));
            REQUIRE(MapBlock::index({-17, -18, -19}) == Chunk::index({3, 2, 1}));
        }
    }

    SECTION("Chunk") {
        SECTION("Relative") {
            SECTION("To Map Block") {
                REQUIRE(Chunk::relative::toMapBlock({0, 0, 0}) == ivec3 {0, 0, 0});
                REQUIRE(Chunk::relative::toMapBlock({0, 5, 0}) == ivec3 {0, 1, 0});
                REQUIRE(Chunk::relative::toMapBlock({5, 20, 0}) == ivec3 {1, 0, 0});
                REQUIRE(Chunk::relative::toMapBlock({40, 20, 30}) == ivec3 {0, 0, 2});
                REQUIRE(Chunk::relative::toMapBlock({0, -5, 0}) == ivec3 {0, 3, 0});
                REQUIRE(Chunk::relative::toMapBlock({-10, -7, -3}) == ivec3 {2, 1, 1});
                REQUIRE(Chunk::relative::toMapBlock({-16, 0, 0}) == ivec3 {0, 0, 0});
                REQUIRE(Chunk::relative::toMapBlock({-17, -18, -19}) == ivec3 {3, 2, 1});
            }

            SECTION("To Region") {
                REQUIRE(Chunk::relative::toRegion({0, 0, 0}) == ivec3 {0, 0, 0});
                REQUIRE(Chunk::relative::toRegion({0, 5, 0}) == ivec3 {0, 5, 0});
                REQUIRE(Chunk::relative::toRegion({5, 20, 0}) == ivec3 {5, 4, 0});
                REQUIRE(Chunk::relative::toRegion({40, 20, 30}) == ivec3 {8, 4, 14});
                REQUIRE(Chunk::relative::toRegion({0, -5, 0}) == ivec3 {0, 11, 0});
                REQUIRE(Chunk::relative::toRegion({-10, -7, -3}) == ivec3 {6, 9, 13});
                REQUIRE(Chunk::relative::toRegion({-16, 0, 0}) == ivec3 {0, 0, 0});
                REQUIRE(Chunk::relative::toRegion({-17, -18, -19}) == ivec3 {15, 14, 13});
            }
        }

        SECTION("World") {
            SECTION("From Block") {
                REQUIRE(Chunk::world::fromBlock({0, 0, 0}) == ivec3 {0, 0, 0});
                REQUIRE(Chunk::world::fromBlock({5, 17, 8}) == ivec3 {0, 1, 0});
                REQUIRE(Chunk::world::fromBlock({23, 0, 3}) == ivec3 {1, 0, 0});
                REQUIRE(Chunk::world::fromBlock({0, 0, 32}) == ivec3 {0, 0, 2});
                REQUIRE(Chunk::world::fromBlock({-4, 40, 0}) == ivec3 {-1, 2, 0});
                REQUIRE(Chunk::world::fromBlock({-17, -16, -15}) == ivec3 {-2, -1, -1});
                REQUIRE(Chunk::world::fromBlock({-35, -30, -64}) == ivec3 {-3, -2, -4});
                REQUIRE(Chunk::world::fromBlock({36, 64, -12}) == ivec3 {2, 4, -1});
            }
        }

        SECTION("Index") {
            REQUIRE(Chunk::index({0, 0, 0}) == 0);
            REQUIRE(Chunk::index({0, 5, 0}) == 4);
            REQUIRE(Chunk::index({5, 20, 0}) == 1);
            REQUIRE(Chunk::index({40, 20, 30}) == 32);
            REQUIRE(Chunk::index({0, -5, 0}) == 12);
            REQUIRE(Chunk::index({-10, -7, -3}) == 22);
            REQUIRE(Chunk::index({-16, 0, 0}) == 0);
            REQUIRE(Chunk::index({3, 3, 3}) == 63);

            REQUIRE(Chunk::index({0, 0, 0}) == Chunk::index({0, 0, 0}));
            REQUIRE(Chunk::index({0, 5, 0}) == Chunk::index({0, 1, 0}));
            REQUIRE(Chunk::index({5, 20, 0}) == Chunk::index({1, 0, 0}));
            REQUIRE(Chunk::index({40, 20, 30}) == Chunk::index({0, 0, 2}));
            REQUIRE(Chunk::index({0, -5, 0}) == Chunk::index({0, 3, 0}));
            REQUIRE(Chunk::index({-10, -7, -3}) == Chunk::index({2, 1, 1}));
            REQUIRE(Chunk::index({-16, 0, 0}) == Chunk::index({0, 0, 0}));
            REQUIRE(Chunk::index({-17, -18, -19}) == Chunk::index({3, 2, 1}));
        }
    }

    SECTION("Block") {
        SECTION("Relative") {
            SECTION("To Chunk") {
                REQUIRE(Block::relative::toChunk({0, 0, 0}) == ivec3 {0, 0, 0});
                REQUIRE(Block::relative::toChunk({0, 5, 0}) == ivec3 {0, 5, 0});
                REQUIRE(Block::relative::toChunk({5, 20, 0}) == ivec3 {5, 4, 0});
                REQUIRE(Block::relative::toChunk({40, 20, 30}) == ivec3 {8, 4, 14});
                REQUIRE(Block::relative::toChunk({0, -5, 0}) == ivec3 {0, 11, 0});
                REQUIRE(Block::relative::toChunk({-10, -7, -3}) == ivec3 {6, 9, 13});
                REQUIRE(Block::relative::toChunk({-16, 0, 0}) == ivec3 {0, 0, 0});
                REQUIRE(Block::relative::toChunk({-17, -18, -19}) == ivec3 {15, 14, 13});
            }

            SECTION("To Map Block") {
                REQUIRE(Block::relative::toMapBlock({0, 0, 0}) == ivec3 {0, 0, 0});
                REQUIRE(Block::relative::toMapBlock({0, 5, 0}) == ivec3 {0, 5, 0});
                REQUIRE(Block::relative::toMapBlock({5, 20, 0}) == ivec3 {5, 20, 0});
                REQUIRE(Block::relative::toMapBlock({40, 20, 30}) == ivec3 {40, 20, 30});
                REQUIRE(Block::relative::toMapBlock({0, -5, 0}) == ivec3 {0, 59, 0});
                REQUIRE(Block::relative::toMapBlock({-10, -7, -3}) == ivec3 {54, 57, 61});
                REQUIRE(Block::relative::toMapBlock({-16, 0, 0}) == ivec3 {48, 0, 0});
                REQUIRE(Block::relative::toMapBlock({-17, -18, -19}) == ivec3 {47, 46, 45});
            }

            SECTION("To Region") {
                REQUIRE(Block::relative::toRegion({0, 0, 0}) == ivec3 {0, 0, 0});
                REQUIRE(Block::relative::toRegion({0, 5, 0}) == ivec3 {0, 5, 0});
                REQUIRE(Block::relative::toRegion({5, 256, 0}) == ivec3 {5, 0, 0});
                REQUIRE(Block::relative::toRegion({40, 20, 30}) == ivec3 {40, 20, 30});
                REQUIRE(Block::relative::toRegion({0, -5, 0}) == ivec3 {0, 251, 0});
                REQUIRE(Block::relative::toRegion({-10, -7, -3}) == ivec3 {246, 249, 253});
                REQUIRE(Block::relative::toRegion({-16, 0, 0}) == ivec3 {240, 0, 0});
                REQUIRE(Block::relative::toRegion({-17, -18, -19}) == ivec3 {239, 238, 237});
            }
        }

        SECTION("Index") {
            REQUIRE(Block::index({0, 0, 0}) == 0);
            REQUIRE(Block::index({0, 5, 0}) == 80);
            REQUIRE(Block::index({5, 20, 0}) == 69);
            REQUIRE(Block::index({40, 20, 30}) == 3656);
            REQUIRE(Block::index({0, -5, 0}) == 176);
            REQUIRE(Block::index({-10, -7, -3}) == 3478);
            REQUIRE(Block::index({-16, 0, 0}) == 0);
            REQUIRE(Block::index({3, 3, 3}) == 819);

            REQUIRE(Block::index({0, 0, 0}) == Block::index({0, 0, 0}));
            REQUIRE(Block::index({0, 17, 0}) == Block::index({0, 1, 0}));
            REQUIRE(Block::index({18, 32, 0}) == Block::index({2, 0, 0}));
            REQUIRE(Block::index({40, 20, 30}) == Block::index({8, 4, 14}));
            REQUIRE(Block::index({0, -5, 0}) == Block::index({0, 11, 0}));
            REQUIRE(Block::index({-10, -7, -3}) == Block::index({6, 9, 13}));
            REQUIRE(Block::index({-16, 0, 0}) == Block::index({0, 0, 0}));
            REQUIRE(Block::index({-17, -18, -19}) == Block::index({15, 14, 13}));
        }
    }
}