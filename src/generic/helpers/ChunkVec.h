//
// Created by aurailus on 24/03/19.
//

#ifndef ZEUS_CHUNKVEC_H
#define ZEUS_CHUNKVEC_H


#include <tgmath.h>
#include "../../client/graphics/mesh/MeshGenerator.h"

class ChunkVec {
public:
    static glm::vec3 roundVec(glm::vec3 vec) {
        return glm::vec3(floor(vec.x), floor(vec.y), floor(vec.z));
    }

    static glm::vec3 chunkVec(glm::vec3 globalVec) {
        return glm::vec3(std::floor(globalVec.x / 16), std::floor(globalVec.y / 16), std::floor(globalVec.z / 16));
    }

    static glm::vec3 localVec(glm::vec3 globalVec) {
        glm::vec3 out;

        if (globalVec.x < 0)
            out.x = 15 + (((int)globalVec.x + 1) % CHUNK_SIZE);
        else
            out.x = ((int)globalVec.x) % CHUNK_SIZE;

        if (globalVec.y < 0)
            out.y = 15 + (((int)globalVec.y + 1) % CHUNK_SIZE);
        else
            out.y = ((int)globalVec.y) % CHUNK_SIZE;

        if (globalVec.z < 0)
            out.z = 15 + (((int)globalVec.z + 1) % CHUNK_SIZE);
        else
            out.z = ((int)globalVec.z) % CHUNK_SIZE;

        return out;
    }
};


#endif //ZEUS_CHUNKVEC_H
