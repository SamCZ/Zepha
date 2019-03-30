//
// Created by aurailus on 29/03/19.
//

#ifndef ZEUS_VECUTILS_H
#define ZEUS_VECUTILS_H


#include <array>
#include <vec3.hpp>

namespace VecUtils {
    static std::array<glm::vec3, 6> getCardinalVectors() {
        return {
            glm::vec3(1, 0, 0), glm::vec3(-1, 0, 0),
            glm::vec3(0, 1, 0), glm::vec3(0, -1, 0),
            glm::vec3(0, 0, 1), glm::vec3(0, 0, -1),
        };
    };
};


#endif //ZEUS_VECUTILS_H
