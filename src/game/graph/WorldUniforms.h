//
// Created by aurailus on 16/06/19.
//

#ifndef ZEUS_WORLDUNIFORMS_H
#define ZEUS_WORLDUNIFORMS_H


#include <GL/gl.h>
#include <mat4x4.hpp>

struct WorldUniforms {
    glm::mat4 matrix;

    GLint proj;
    GLint model;
    GLint view;

    GLint gPosition;
    GLint gNormal;
    GLint gColorSpec;
};


#endif //ZEUS_WORLDUNIFORMS_H
