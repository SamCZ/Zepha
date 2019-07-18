//
// Created by aurailus on 18/06/19.
//

#ifndef ZEUS_ENTITYGEOMETRYUNIFORMS_H
#define ZEUS_ENTITYGEOMETRYUNIFORMS_H


#include <GL/gl.h>
#include <glm/mat4x4.hpp>

struct EntityGeometryUniforms {
    glm::mat4 matrix;

    GLint proj;
    GLint model;
    GLint view;
};

#endif //ZEUS_ENTITYGEOMETRYUNIFORMS_H
