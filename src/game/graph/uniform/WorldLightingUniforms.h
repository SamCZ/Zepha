//
// Created by aurailus on 17/06/19.
//

#pragma once


#include <GL/gl.h>
#include <glm/mat4x4.hpp>

struct WorldLightingUniforms {
    GLint gPosition;
    GLint gNormal;
    GLint gColorSpec;

    GLint camPosition;
};

