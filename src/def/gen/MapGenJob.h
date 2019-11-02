//
// Created by aurailus on 13/02/19.
//

#pragma once

#include <array>
#include <vector>
#include <glm/vec3.hpp>
#include "../../util/TransPos.h"

typedef unsigned int uint;

struct MapGenJob {

    std::array<uint, 4096> blocks;
    std::vector<float> density;
    std::vector<int> depth;
    std::vector<float> depthFloat;

    glm::vec3 pos {};

    explicit MapGenJob(glm::vec3 pos) :
        pos(pos),
        blocks(),
        density(4096),
        depth(4096),
        depthFloat(4096) {}
};
