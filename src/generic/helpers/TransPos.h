//
// Created by aurailus on 24/03/19.
//

#ifndef ZEUS_CHUNKVEC_H
#define ZEUS_CHUNKVEC_H


#include <tgmath.h>
#include "../../client/graphics/mesh/MeshGenerator.h"

class TransPos {
public:
    static glm::vec3 roundPos(glm::vec3 vec) {
        return { floor(vec.x), floor(vec.y), floor(vec.z) };
    }

    static glm::vec3 chunkFromGlobal(glm::vec3 vec) {
        auto round = roundPos(vec);
        return { std::floor(round.x / 16), std::floor(round.y / 16), std::floor(round.z / 16) };
    }

    static glm::vec3 chunkLocalFromGlobal(glm::vec3 vec) {
        auto round = roundPos(vec);
        glm::vec3 out;
        out.x = (round.x < 0) ? ((CHUNK_SIZE - 1) + (( (int)round.x + 1) % CHUNK_SIZE)) : (( (int)round.x) % CHUNK_SIZE);
        out.y = (round.y < 0) ? ((CHUNK_SIZE - 1) + (( (int)round.y + 1) % CHUNK_SIZE)) : (( (int)round.y) % CHUNK_SIZE);
        out.z = (round.z < 0) ? ((CHUNK_SIZE - 1) + (( (int)round.z + 1) % CHUNK_SIZE)) : (( (int)round.z) % CHUNK_SIZE);
        return out;
    }
};


#endif //ZEUS_CHUNKVEC_H
