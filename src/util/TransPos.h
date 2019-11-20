//
// Created by aurailus on 24/03/19.
//

#pragma once


#include <tgmath.h>
#include <glm/vec3.hpp>

namespace TransPos {
    const static int CHUNK_SIZE = 16; //Must be a multiple of 16
    const static int MAPBLOCK_SIZE = 4;
    const static int REGION_SIZE = 4;

    const static int CHUNK_BLOCK_LENGTH = CHUNK_SIZE;
    const static int MAPBLOCK_BLOCK_LENGTH = CHUNK_BLOCK_LENGTH * MAPBLOCK_SIZE;
    const static int REGION_BLOCK_LENGTH = MAPBLOCK_BLOCK_LENGTH * REGION_SIZE;

    const static int MAPBLOCK_CHUNK_LENGTH = MAPBLOCK_SIZE;
    const static int REGION_CHUNK_LENGTH = MAPBLOCK_CHUNK_LENGTH * REGION_SIZE;

    static inline glm::vec3 roundPos(glm::vec3 vec) {
        return { floor(vec.x), floor(vec.y), floor(vec.z) };
    }

    namespace {
        inline glm::vec3 localFromGlobal(glm::vec3 &pos, int size) {
            auto round = roundPos(pos);
            glm::vec3 out;
            out.x = (round.x < 0) ? ((size - 1) + (((int) round.x + 1) % size)) : (((int) round.x) % size);
            out.y = (round.y < 0) ? ((size - 1) + (((int) round.y + 1) % size)) : (((int) round.y) % size);
            out.z = (round.z < 0) ? ((size - 1) + (((int) round.z + 1) % size)) : (((int) round.z) % size);
            return out;
        }

        inline glm::vec3 sectionFromGlobal(glm::vec3 pos, int size) {
            auto round = roundPos(pos);
            return {std::floor(round.x / size), std::floor(round.y / size), std::floor(round.z / size)};
        }
    }



    //Return a local position vector relative to the respective structure

    static inline glm::vec3 regionLocalFromVec(glm::vec3 vec) {
        return localFromGlobal(vec, REGION_BLOCK_LENGTH);
    }

    static inline glm::vec3 mapBlockLocalFromVec(glm::vec3 vec) {
        return localFromGlobal(vec, MAPBLOCK_BLOCK_LENGTH);
    }

    static inline glm::vec3 chunkLocalFromVec(glm::vec3 vec) {
        return localFromGlobal(vec, CHUNK_BLOCK_LENGTH);
    }

    //Returns the Chunk Position from a pos vector

    static inline glm::vec3 chunkFromVec(glm::vec3 vec) {
        return sectionFromGlobal(vec, CHUNK_BLOCK_LENGTH);
    }

    static inline glm::vec3 mapBlockFromVec(glm::vec3 vec) {
        return sectionFromGlobal(vec, MAPBLOCK_BLOCK_LENGTH);
    }

    namespace Dimension {
        //Used to get the chunk position from a global coordinate in LocalDimension

        //Retrieve the region pos from a block pos
        static inline glm::vec3 regionFromVec(glm::vec3 vec) {
            return sectionFromGlobal(vec, REGION_CHUNK_LENGTH);
        }

        //Retrieve the raw (chunk pos of bottom left corner) pos of a region
        static inline glm::vec3 regionRawFromRegionVec(glm::vec3 vec) {
            return vec * glm::vec3(REGION_CHUNK_LENGTH);
        }

        //Retrieve the offset of a mapblock from it's region using a chunk position
        static inline glm::vec3 mapBlockOffsetFromRegion(glm::vec3 vec) {
            return sectionFromGlobal(localFromGlobal(vec, REGION_CHUNK_LENGTH), MAPBLOCK_CHUNK_LENGTH);
        }

        //Retrieve the offset of a chunk from it's mapblock using a chunk position
        static inline glm::vec3 chunkOffsetFromMapBlock(glm::vec3 vec) {
            return localFromGlobal(vec, MAPBLOCK_CHUNK_LENGTH);
        }
    }



    // Used to get indices from various vectors

    static inline unsigned int mapBlockIndFromVec(glm::vec3 vec) {
        return (unsigned int)(vec.x + REGION_SIZE * (vec.y + REGION_SIZE * vec.z));
    }

    static inline unsigned int chunkIndFromVec(glm::vec3 vec) {
        return (unsigned int)(vec.x + MAPBLOCK_SIZE * (vec.y + MAPBLOCK_SIZE * vec.z));
    }
};

