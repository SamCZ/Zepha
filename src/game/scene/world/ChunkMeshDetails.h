//
// Created by aurailus on 23/07/19.
//

#pragma once

#include <vector>
#include <glm/vec3.hpp>

#include "../../graph/meshtypes/ChunkVertex.h"

struct ChunkMeshDetails {
    std::vector<ChunkVertex> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 pos {0, 0, 0};
};
