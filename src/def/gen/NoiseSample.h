//
// Created by aurailus on 15/02/19.
//

#pragma once

#include <vector>
#include <iostream>
#include <noise/noise.h>
#include <glm/glm.hpp>
#include <glm/common.hpp>
#include "../../util/Interp.h"

class NoiseSample {
public:
    NoiseSample() = default;
    explicit NoiseSample(glm::ivec2 precision);
    NoiseSample(noise::module::Module& module, glm::vec3 pos, glm::ivec2 precision, bool flat = false);

    void set(glm::ivec3 pos, float value);
    float get(const glm::ivec3& pos);
private:
    void reserveSpace();

    glm::ivec2 prec {};
    std::vector<std::vector<std::vector<float>>> data {};
};

