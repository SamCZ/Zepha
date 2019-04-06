//
// Created by aurailus on 24/03/19.
//

#ifndef ZEUS_CHUNKVEC_H
#define ZEUS_CHUNKVEC_H


#include <tgmath.h>
#include <vec3.hpp>

namespace TransPos {
    const static int CHUNK_SIZE = 16;
    const static int MAPBLOCK_SIZE = 8;
    const static int REGION_SIZE = 8;

    const static int CHUNK_LENGTH = CHUNK_SIZE;
    const static int MAPBLOCK_LENGTH = CHUNK_SIZE * MAPBLOCK_SIZE;
    const static int REGION_LENGTH = CHUNK_SIZE * MAPBLOCK_SIZE * REGION_SIZE;

    static glm::vec3 roundPos(glm::vec3 vec) {
        return { floor(vec.x), floor(vec.y), floor(vec.z) };
    }

    namespace {
        glm::vec3 localFromGlobal(glm::vec3 &pos, int size) {
            auto round = roundPos(pos);
            glm::vec3 out;
            out.x = (round.x < 0) ? ((size - 1) + (((int) round.x + 1) % size)) : (((int) round.x) % size);
            out.y = (round.y < 0) ? ((size - 1) + (((int) round.y + 1) % size)) : (((int) round.y) % size);
            out.z = (round.z < 0) ? ((size - 1) + (((int) round.z + 1) % size)) : (((int) round.z) % size);
            return out;
        }

        glm::vec3 sectionFromGlobal(glm::vec3 &pos, int size) {
            auto round = roundPos(pos);
            return {std::floor(round.x / size), std::floor(round.y / size), std::floor(round.z / size)};
        }
    }

    //Region Functions

    static glm::vec3 regionFromVec(glm::vec3 vec) {
        return sectionFromGlobal(vec, REGION_LENGTH);
    }

    //Retrieve the local chunk position in a region from a global chunk position
    static glm::vec3 regionLocalFromVec(glm::vec3 vec) {
        return localFromGlobal(vec, REGION_LENGTH);
    }

    //
    //Mapblock Functions
    //


    //Retrieve the mapblock of a chunk vector from a region local chunk position
    static glm::vec3 mapBlockFromVec(glm::vec3 vec) {
        return sectionFromGlobal(vec, MAPBLOCK_SIZE);
    }

    //Retrieve the chunk's offset from the mapblock it is in
    static glm::vec3 mapBlockLocalFromVec(glm::vec3 vec) {
        return localFromGlobal(vec, MAPBLOCK_SIZE);
    }

    //Retrieve the offset of a mapblock from it's region using a chunk position
    static glm::vec3 mapBlockOffsetFromRegion(glm::vec3 vec) {
        return mapBlockFromVec(regionLocalFromVec(vec));
    }

    static unsigned int mapBlockIndFromVec(glm::vec3 vec) {
        return (unsigned int)(vec.x + REGION_SIZE * (vec.y + REGION_SIZE * vec.z));
    }

    //Chunk Functions

    static glm::vec3 chunkFromVec(glm::vec3 vec) {
        return sectionFromGlobal(vec, CHUNK_LENGTH);
    }

    static glm::vec3 chunkLocalFromVec(glm::vec3 vec) {
        return localFromGlobal(vec, CHUNK_LENGTH);
    }

    //Retrieve the offset of a chunk from it's mapblock using a chunk position
    static glm::vec3 chunkOffsetFromBlock(glm::vec3 vec) {
        return mapBlockLocalFromVec(vec);
    }

    static unsigned int chunkIndFromVec(glm::vec3 vec) {
        return (unsigned int)(vec.x + MAPBLOCK_SIZE * (vec.y + MAPBLOCK_SIZE * vec.z));
    }
};


#endif //ZEUS_CHUNKVEC_H
