//
// Created by aurailus on 2020-02-01.
//

#pragma once

#include <vector>
#include <glm/vec3.hpp>

struct Schematic {
    std::vector<unsigned int> blocks {};
    glm::ivec3 dimensions {};
    glm::ivec3 origin {};

//    inline unsigned int index(const glm::ivec3& vec) {
//        return static_cast<unsigned int>(vec.x + dimensions.x * (vec.y + dimensions.y * vec.z));
//    }

    inline unsigned int length() {
        return blocks.size();
    }

    inline void assignOffset(int ind, glm::ivec3& vec) {
        vec.z = ind / (dimensions.x * dimensions.y);
        ind -= ((int)vec.z * dimensions.x * dimensions.y);
        vec.y = ind / dimensions.y;
        vec.x = ind % dimensions.z;
    }
};
