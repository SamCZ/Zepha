//
// Created by aurailus on 29/03/19.
//

#pragma once

#include <array>
#include <glm/vec3.hpp>
#include <cstddef>
#include <functional>
#include "Space.h"

namespace Vec {
    struct compareFunc {
        size_t operator()(const glm::vec3& k)const {
            return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
        }
    };

    const static std::array<glm::vec3, 6> cardinalVectors = {
            glm::vec3 {1, 0, 0}, glm::vec3 {-1, 0, 0},
            glm::vec3 {0, 1, 0}, glm::vec3 {0, -1, 0},
            glm::vec3 {0, 0, 1}, glm::vec3 {0, 0, -1}
    };

    static inline void indAssignVec(int ind, glm::vec3& vec) {
        vec.z = ind / (16 * 16);
        ind -= ((int)vec.z * 16 * 16);
        vec.y = ind / 16;
        vec.x = ind % 16;
    }

    static inline glm::vec3* indToVec(int ind) {
        int z = ind / (16 * 16);
        ind -= (z * 16 * 16);
        int y = ind / 16;
        int x = ind % 16;
        return new glm::vec3(x, y, z);
    }
};

