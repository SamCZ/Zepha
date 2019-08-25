//
// Created by aurailus on 02/12/18.
//

#pragma once

#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

struct BlockModelVertex {
    glm::vec3 pos;
    glm::vec3 nml;
    glm::vec2 tex;
    glm::vec2 texUVs;
};
