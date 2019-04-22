//
// Created by aurailus on 29/03/19.
//

#ifndef ZEUS_VECUTILS_H
#define ZEUS_VECUTILS_H

#include <array>
#include <vec3.hpp>
#include <cstddef>
#include <functional>
#include "TransPos.h"

namespace VecUtils {
    struct compareFunc {
        size_t operator()(const glm::vec3& k)const {
            return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
        }
    };

    static inline std::array<glm::vec3, 6> getCardinalVectors() {
        return {
            glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0),
            glm::vec3(0, 1, 0), glm::vec3(0, -1, 0),
            glm::vec3(0, 0, 1), glm::vec3(0, 0, -1),
        };
    };

    static inline void indAssignVec(int ind, glm::vec3& vec) {
        vec.z = ind / (TransPos::CHUNK_SIZE * TransPos::CHUNK_SIZE);
        ind -= ((int)vec.z * TransPos::CHUNK_SIZE * TransPos::CHUNK_SIZE);
        vec.y = ind / TransPos::CHUNK_SIZE;
        vec.x = ind % TransPos::CHUNK_SIZE;
    }

    static inline glm::vec3* indToVec(int ind) {
        int z = ind / (TransPos::CHUNK_SIZE * TransPos::CHUNK_SIZE);
        ind -= (z * TransPos::CHUNK_SIZE * TransPos::CHUNK_SIZE);
        int y = ind / TransPos::CHUNK_SIZE;
        int x = ind % TransPos::CHUNK_SIZE;
        return new glm::vec3(x, y, z);
    }

    static inline unsigned int vecToInd(int x, int y, int z) {
        return (unsigned int)(x + TransPos::CHUNK_SIZE * (y + TransPos::CHUNK_SIZE * z));
    }

    static inline unsigned int vecToInd(glm::vec3* vec) {
        return vecToInd((int)vec->x, (int)vec->y, (int)vec->z);
    }
};


#endif //ZEUS_VECUTILS_H
