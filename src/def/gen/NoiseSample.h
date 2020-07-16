//
// Created by aurailus on 15/02/19.
//

#pragma once

#include <vector>
#include <functional>
#include <glm/glm.hpp>

#include "../../util/Interp.h"

class NoiseSample {
public:
    typedef std::function<float(glm::vec3 pos)> fill_function;

    NoiseSample(unsigned int precision, float scaleBy = 1);
    NoiseSample(glm::ivec2 precision, glm::vec2 scaleBy = {1, 1});

    void populate(const fill_function& fn);

    inline float get(glm::vec3 pos) {
        glm::vec3 scaled = pos * glm::vec3(precision) / scaleBy;

        glm::ivec3 a = glm::ivec3(scaled);
        glm::vec3 factor = scaled - glm::floor(scaled);
        glm::ivec3 b = {fmin(a.x + ceil(factor.x), precision.x), fmin(a.y + ceil(factor.y), precision.y), fmin(a.z + ceil(factor.z), precision.z)};

//        assert(a.x + factor.x <= precision.x && a.y + factor.y <= precision.y && a.z + factor.z <= precision.z);

        // No vertical interpolation
        if (precision.y == 0) return Interp::bilerp(
            data[index(a.x, a.y, a.z)], data[index(b.x, a.y, a.z)],
            data[index(a.x, a.y, b.z)], data[index(b.x, a.y, b.z)],
            factor.x, factor.z);

        return Interp::trilerp(
            data[index(a.x, a.y, a.z)], data[index(b.x, a.y, a.z)],
            data[index(a.x, a.y, b.z)], data[index(b.x, a.y, b.z)],
            data[index(a.x, b.y, a.z)], data[index(b.x, b.y, a.z)],
            data[index(a.x, b.y, b.z)], data[index(b.x, b.y, b.z)],
            factor.x, factor.z, factor.y);
    }
private:
    inline unsigned int index(int x, int y, int z) {
        return x * (precision.x + 1) * (precision.y + 1) + y * (precision.x + 1) + z;
    };

    std::vector<float> data {};
    glm::ivec3 precision {};
    glm::vec3 scaleBy;
};

