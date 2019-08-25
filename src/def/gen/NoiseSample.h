//
// Created by aurailus on 15/02/19.
//

#pragma once

#include <vector>
#include <iostream>
#include <glm/vec3.hpp>
#include <noise/noise.h>

class NoiseSample {
public:
    static NoiseSample getSample(noise::module::Module *module, glm::vec3 chunkPos, int hPrecision = 8, int vPrecision = 8, bool flat = false);

    NoiseSample(int hPrecision, int vPrecision);

    void  set(glm::vec3 pos, float value);
    float get(glm::vec3& pos);

private:
    std::vector<std::vector<std::vector<float>>> data;

    int hPrecision;
    int vPrecision;
};

