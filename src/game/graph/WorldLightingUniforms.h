//
// Created by aurailus on 17/06/19.
//

#ifndef ZEUS_WORLDLIGHTINGUNIFORMS_H
#define ZEUS_WORLDLIGHTINGUNIFORMS_H


#include <GL/gl.h>
#include <mat4x4.hpp>

struct WorldLightingUniforms {
    GLint gPosition;
    GLint gNormal;
    GLint gColorSpec;

    GLint camPosition;
};


#endif //ZEUS_WORLDLIGHTINGUNIFORMS_H
