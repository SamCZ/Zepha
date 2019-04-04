//
// Created by aurailus on 29/03/19.
//

#ifndef ZEUS_VECUTILS_H
#define ZEUS_VECUTILS_H

#include <array>
#include <vec3.hpp>
#include <cstddef>
#include <functional>

namespace VecUtils {
    struct compareFunc {
        size_t operator()(const glm::vec3& k)const {
            return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
        }
    };

    static std::array<glm::vec3, 6> getCardinalVectors() {
        return {
            glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0),
            glm::vec3(0, 1, 0), glm::vec3(0, -1, 0),
            glm::vec3(0, 0, 1), glm::vec3(0, 0, -1),
        };
    };
};


#endif //ZEUS_VECUTILS_H
