//
// Created by aurailus on 13/02/19.
//

#pragma once

#include <array>
#include <glm/vec3.hpp>
#include "NoiseSample.h"

class MapGenJob {
public:
    std::array<float, 4096> density {};
    std::array<float, 4096> depth {};

    NoiseSample temperature, humidity, roughness;
};
