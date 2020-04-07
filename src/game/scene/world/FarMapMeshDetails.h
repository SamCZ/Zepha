//
// Created by aurailus on 2020-04-06.
//

#pragma once

#include <vector>
#include <glm/vec3.hpp>

struct FarMapMeshDetails {
//    std::vector<FarMapVertex> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 pos {0, 0, 0};
};
