//
// Created by aurailus on 2020-04-05.
//

#pragma once

#include <vector>
#include <glm/vec3.hpp>
#include "NoiseSample.h"

struct FarMapJob {
    std::vector<float> density {};
    std::vector<float> depth {};

    unsigned short downScale;

    NoiseSample temperature, humidity, roughness;
};