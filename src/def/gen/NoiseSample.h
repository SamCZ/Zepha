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

        glm::vec3 a = glm::floor(scaled);
        glm::vec3 factor = scaled - glm::floor(scaled);
        glm::vec3 b = {fmin(a.x + ceil(factor.x), precision.x), fmin(a.y + ceil(factor.y), precision.y), fmin(a.z + ceil(factor.z), precision.z)};

        assert(a.x + factor.x <= precision.x && a.y + factor.y <= precision.y && a.z + factor.z <= precision.z);

        const auto& p00 = data[a.y][a.x];
        const auto& p10 = data[a.y][b.x];

        // No vertical interpolation
        if (precision.y == 0) return Interp::bilerp(p00[a.z], p10[a.z], p00[b.z], p10[b.z], factor.x, factor.z);

        const auto& p01 = data[b.y][a.x];
        const auto& p11 = data[b.y][b.x];

        return Interp::trilerp(p00[a.z], p10[a.z], p00[b.z], p10[b.z],
            p01[a.z], p11[a.z], p01[b.z], p11[b.z], factor.x, factor.z, factor.y);
    }
private:
    unsigned int length;

    std::vector<std::vector<std::vector<float>>> data {};
    glm::ivec3 precision {};
    glm::vec3 scaleBy;
};

