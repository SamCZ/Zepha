//
// Created by aurailus on 09/09/19.
//

#pragma once

#include <GL/gl.h>
#include <glm/mat4x4.hpp>

struct SSAOUniforms {
    glm::mat4 matrix;

    GLint proj;
//    GLint model;
    GLint view;

//    GLint swaySampler;

//    GLint time;
};

