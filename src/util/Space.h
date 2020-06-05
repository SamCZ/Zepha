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
        inline glm::ivec3 localFromGlobal(const glm::ivec3 &pos, int size) {
            return glm::vec3 {
                (pos.x < 0) ? size - 1 + static_cast<int>(pos.x + 1) % size : static_cast<int>(pos.x) % size,
                (pos.y < 0) ? size - 1 + static_cast<int>(pos.y + 1) % size : static_cast<int>(pos.y) % size,
                (pos.z < 0) ? size - 1 + static_cast<int>(pos.z + 1) % size : static_cast<int>(pos.z) % size
            };
        }

        inline glm::ivec3 sectionFromGlobal(glm::ivec3 pos, int size) {
            return {
                std::floor(static_cast<float>(pos.x) / size),
                std::floor(static_cast<float>(pos.y) / size),
                std::floor(static_cast<float>(pos.z) / size)};
        }
    }

    namespace Region {
        namespace world {
            // Get a Region engine position from a MapBlock's world position.
            static inline glm::ivec3 fromMapBlock(const glm::ivec3& mapBlock) {
                return sectionFromGlobal(mapBlock, REGION_SIZE);
            }

            // Get a Region engine position from a Chunk's world position.
            static inline glm::ivec3 fromChunk(const glm::ivec3 &chunk) {
                return sectionFromGlobal(chunk, REGION_CHUNK_LENGTH);
            }

            // Get a Region engine position from a Block's world position.
            static inline glm::ivec3 fromBlock(const glm::ivec3 &chunk) {
                return sectionFromGlobal(chunk, REGION_BLOCK_LENGTH);
            }
        }
    }

    namespace MapBlock {
        namespace relative {
            // Get a MapBlock's relative position in its Region from its world position.
            static inline glm::ivec3 toRegion(const glm::ivec3& pos) {
                return localFromGlobal(pos, MAPBLOCK_SIZE);
            }
        }

        namespace world {
            // Get a MapBlock engine position from a Chunk's world position.
            static inline glm::ivec3 fromChunk(const glm::ivec3 &chunk) {
                return sectionFromGlobal(chunk, MAPBLOCK_SIZE);
            }

            // Get a MapBlock engine position from a Block's world position.
            static inline glm::ivec3 fromBlock(const glm::ivec3 &vec) {
                return sectionFromGlobal(vec, MAPBLOCK_BLOCK_LENGTH);
            }
        }

        // Get the index of a MapBlock within its Region from its local or world position.
        static inline unsigned int index(const glm::ivec3& vec) {
            glm::ivec3 local = MapBlock::relative::toRegion(vec);
            unsigned int ind = static_cast<unsigned int>(local.x + REGION_SIZE * (local.y + REGION_SIZE * local.z));
            return ind;
        }
    }

    namespace Chunk {
        namespace relative {
            // Get a Chunk's relative position in its MapBlock from its world position.
            static inline glm::ivec3 toMapBlock(const glm::ivec3& pos) {
                return localFromGlobal(pos, MAPBLOCK_CHUNK_LENGTH);
            }

            // Get a Chunk's relative position in its Region from its world position.
            static inline glm::ivec3 toRegion(const glm::ivec3& pos) {
                return localFromGlobal(pos, REGION_CHUNK_LENGTH);
            }
        }

        namespace world {
            // Get a Chunk world position from a Block's world position.
            static inline glm::ivec3 fromBlock(const glm::ivec3& pos) {
                return sectionFromGlobal(pos, CHUNK_BLOCK_LENGTH);
            }
        }

        // Get the index of a Chunk within its MapBlock from its local or world position.
        static inline unsigned int index(const glm::ivec3& vec) {
            glm::ivec3 local = Chunk::relative::toMapBlock(vec);
            return static_cast<unsigned int>(local.x + MAPBLOCK_SIZE * (local.z + MAPBLOCK_SIZE * local.y));
        }

        // Return a local vector of an chunk within its mapblock.
        static inline glm::ivec3 fromIndex(unsigned int ind) {
            glm::ivec3 vec {};

            vec.y = ind / (MAPBLOCK_SIZE * MAPBLOCK_SIZE);
            ind -= (static_cast<int>(vec.y) * MAPBLOCK_SIZE * MAPBLOCK_SIZE);
            vec.z = ind / MAPBLOCK_SIZE;
            vec.x = ind % MAPBLOCK_SIZE;

            return vec;
        }
    }

    namespace Block {
        namespace relative {
            // Get a Block's relative position to its Chunk from its world position.
            static inline glm::ivec3 toChunk(const glm::ivec3& pos) {
                return localFromGlobal(pos, CHUNK_BLOCK_LENGTH);
            }

            // Get a Block's relative position to its MapBlock from its world position.
            static inline glm::ivec3 toMapBlock(const glm::ivec3& pos) {
                return localFromGlobal(pos, MAPBLOCK_BLOCK_LENGTH);
            }

            // Get a Block's relative position in its Region from its world position.
            static inline glm::ivec3 toRegion(const glm::ivec3& pos) {
                return localFromGlobal(pos, REGION_BLOCK_LENGTH);
            }
        }

        // Get the index of a Block within its Chunk from its local or world position.
        static inline unsigned int index(const glm::ivec3& vec) {
            glm::ivec3 local = Block::relative::toChunk(vec);
            return static_cast<unsigned int>(local.x + CHUNK_SIZE * (local.z + CHUNK_SIZE * local.y));
        }

        // Return a local vector of an block within its chunk.
        static inline glm::ivec3 fromIndex(unsigned int ind) {
            glm::ivec3 vec {};

            vec.y = ind / (CHUNK_SIZE * CHUNK_SIZE);
            ind -= (static_cast<int>(vec.y) * CHUNK_SIZE * CHUNK_SIZE);
            vec.z = ind / CHUNK_SIZE;
            vec.x = ind % CHUNK_SIZE;

            return vec;
        }
    }
}

