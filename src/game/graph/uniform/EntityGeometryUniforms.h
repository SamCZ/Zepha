//
// Created by aurailus on 18/06/19.
//

#pragma once


#include <GL/gl.h>
#include <glm/mat4x4.hpp>

struct EntityGeometryUniforms {
    glm::mat4 matrix;

    GLint proj;
    GLint model;
    GLint view;

    GLint uBones;
};
