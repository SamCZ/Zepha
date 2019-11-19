//
// Created by aurailus on 13/02/19.
//

#pragma once

#include <array>
#include <vector>
#include <glm/vec3.hpp>
#include "../../util/TransPos.h"

struct MapGenJob {
    std::array<unsigned int, 4096> blocks;
    std::array<unsigned short, 4096> biomes;
    std::array<float, 4096> density;
    std::array<float, 4096> depth;

    glm::vec3 pos {};

    explicit MapGenJob(glm::vec3 pos) :
            pos(pos),
            blocks(),
            biomes(),
            density(),
            depth() {}
};
