//
// Created by aurailus on 02/12/18.
//

#ifndef GLPROJECT_VERTEX_H
#define GLPROJECT_VERTEX_H

#include <vec3.hpp>
#include <vec2.hpp>

struct MeshVertex {
    glm::vec3 pos;
    glm::vec3 nml;
    glm::vec2 tex;
    glm::vec2 texUVs;
};


#endif //GLPROJECT_VERTEX_H