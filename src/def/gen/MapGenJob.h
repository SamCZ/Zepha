//
// Created by aurailus on 13/02/19.
//

#pragma once

#include <array>
#include <vector>
#include <glm/vec3.hpp>
#include "../../util/TransPos.h"

struct MapGenJob {

    std::array<uint, 4096> blocks;
    std::vector<float> density;
    std::vector<int> depth;
    std::vector<float> depthFloat;

    glm::vec3 pos {};

    explicit MapGenJob(glm::vec3 pos) {

        this->pos = pos;

        blocks = std::array<uint, 4096>();
        density = std::vector<float>((unsigned long)pow(TransPos::CHUNK_SIZE, 3));
        depth = std::vector<int>((unsigned long)pow(TransPos::CHUNK_SIZE, 3));
        depthFloat = std::vector<float>((unsigned long)pow(TransPos::CHUNK_SIZE, 3));
    }
};
