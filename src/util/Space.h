//
// Created by aurailus on 24/03/19.
//

#pragma once


#include <tgmath.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>

namespace Space {
    const static int CHUNK_SIZE = 16;
    const static int MAPBLOCK_SIZE = 4;
    const static int REGION_SIZE = 4;

    const static int CHUNK_BLOCK_LENGTH = CHUNK_SIZE;
    const static int MAPBLOCK_BLOCK_LENGTH = CHUNK_BLOCK_LENGTH * MAPBLOCK_SIZE;
    const static int REGION_BLOCK_LENGTH = MAPBLOCK_BLOCK_LENGTH * REGION_SIZE;

    const static int MAPBLOCK_CHUNK_LENGTH = MAPBLOCK_SIZE;
    const static int REGION_CHUNK_LENGTH = MAPBLOCK_CHUNK_LENGTH * REGION_SIZE;

    // Private helper methods
    namespace {
        inline glm::vec3 localFromGlobal(const glm::vec3 &pos, int size) {
            auto round = glm::floor(pos);
            return glm::vec3 {
                (round.x < 0) ? size - 1 + static_cast<int>(round.x + 1) % size : static_cast<int>(round.x) % size,
                (round.y < 0) ? size - 1 + static_cast<int>(round.y + 1) % size : static_cast<int>(round.y) % size,
                (round.z < 0) ? size - 1 + static_cast<int>(round.z + 1) % size : static_cast<int>(round.z) % size
            };
        }

        inline glm::vec3 sectionFromGlobal(glm::vec3 pos, int size) {
            auto round = glm::floor(pos);
            return {std::floor(round.x / size), std::floor(round.y / size), std::floor(round.z / size)};
        }
    }

    namespace Region {
        namespace world {
            // Get a Region world position from a MapBlock's world position.
            static inline glm::vec3 fromMapBlock(const glm::vec3& mapBlock) {
                return sectionFromGlobal(mapBlock, REGION_SIZE);
            }

            // Get a Region world position from a Chunk's world position.
            static inline glm::vec3 fromChunk(const glm::vec3 &chunk) {
                return sectionFromGlobal(chunk, REGION_CHUNK_LENGTH);
            }

            // Get a Region world position from a Block's world position.
            static inline glm::vec3 fromBlock(const glm::vec3 &chunk) {
                return sectionFromGlobal(chunk, REGION_BLOCK_LENGTH);
            }
        }
    }

    namespace MapBlock {
        namespace relative {
            // Get a MapBlock's relative position in its Region from its world position.
            static inline glm::vec3 toRegion(const glm::vec3& pos) {
                return localFromGlobal(pos, MAPBLOCK_SIZE);
            }
        }

        namespace world {
            // Get a MapBlock world position from a Chunk's world position.
            static inline glm::vec3 fromChunk(const glm::vec3 &chunk) {
                return sectionFromGlobal(chunk, MAPBLOCK_SIZE);
            }

            // Get a MapBlock world position from a Block's world position.
            static inline glm::vec3 fromBlock(const glm::vec3 &vec) {
                return sectionFromGlobal(vec, MAPBLOCK_BLOCK_LENGTH);
            }
        }

        // Get the index of a MapBlock within its Region from its local or world position.
        static inline unsigned int index(const glm::vec3& vec) {
            glm::vec3 local = MapBlock::relative::toRegion(vec);
            unsigned int ind = static_cast<unsigned int>(local.x + REGION_SIZE * (local.y + REGION_SIZE * local.z));
            return ind;
        }
    }

    namespace Chunk {
        namespace relative {
            // Get a Chunk's relative position in its MapBlock from its world position.
            static inline glm::vec3 toMapBlock(const glm::vec3& pos) {
                return localFromGlobal(pos, MAPBLOCK_CHUNK_LENGTH);
            }

            // Get a Chunk's relative position in its Region from its world position.
            static inline glm::vec3 toRegion(const glm::vec3& pos) {
                return localFromGlobal(pos, REGION_CHUNK_LENGTH);
            }
        }

        namespace world {
            // Get a Chunk world position from a Block's world position.
            static inline glm::vec3 fromBlock(const glm::vec3& pos) {
                return sectionFromGlobal(pos, CHUNK_BLOCK_LENGTH);
            }
        }

        // Get the index of a Chunk within its MapBlock from its local or world position.
        static inline unsigned int index(const glm::vec3& vec) {
            glm::vec3 local = Chunk::relative::toMapBlock(vec);
            return static_cast<unsigned int>(local.x + MAPBLOCK_SIZE * (local.y + MAPBLOCK_SIZE * local.z));
        }
    }

    namespace Block {
        namespace relative {
            // Get a Block's relative position to its Chunk from its world position.
            static inline glm::vec3 toChunk(const glm::vec3& pos) {
                return localFromGlobal(pos, CHUNK_BLOCK_LENGTH);
            }

            // Get a Block's relative position to its MapBlock from its world position.
            static inline glm::vec3 toMapBlock(const glm::vec3& pos) {
                return localFromGlobal(pos, MAPBLOCK_BLOCK_LENGTH);
            }

            // Get a Block's relative position in its Region from its world position.
            static inline glm::vec3 toRegion(const glm::vec3& pos) {
                return localFromGlobal(pos, REGION_BLOCK_LENGTH);
            }
        }

        // Get the index of a Block within its Chunk from its local or world position.
        static inline unsigned int index(const glm::vec3& vec) {
            glm::vec3 local = Block::relative::toChunk(vec);
            return static_cast<unsigned int>(local.x + CHUNK_SIZE * (local.y + CHUNK_SIZE * local.z));
        }
    }
}

