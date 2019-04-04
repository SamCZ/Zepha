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

    static glm::vec3 regionFromGlobal(glm::vec3 vec) {
        return sectionFromGlobal(vec, REGION_LENGTH);
    }

    static glm::vec3 regionLocalFromGlobal(glm::vec3 vec) {
        return localFromGlobal(vec, REGION_LENGTH);
    }

    static glm::vec3 mapBlockFromGlobal(glm::vec3 vec) {
        return sectionFromGlobal(vec, MAPBLOCK_LENGTH);
    }

    static glm::vec3 mapBlockLocalFromGlobal(glm::vec3 vec) {
        return localFromGlobal(vec, MAPBLOCK_LENGTH);
    }

    static glm::vec3 chunkFromGlobal(glm::vec3 vec) {
        return sectionFromGlobal(vec, CHUNK_LENGTH);
    }

    static glm::vec3 chunkLocalFromGlobal(glm::vec3 vec) {
        return localFromGlobal(vec, CHUNK_LENGTH);
    }
};


#endif //ZEUS_CHUNKVEC_H
