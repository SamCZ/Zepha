//
// Created by aurailus on 16/06/19.
//

#ifndef ZEUS_WORLDUNIFORMS_H
#define ZEUS_WORLDUNIFORMS_H


#include <GL/gl.h>
#include <glm/mat4x4.hpp>

struct WorldGeometryUniforms {
    glm::mat4 matrix;

    GLint proj;
    GLint model;
    GLint view;



    GLint time;
};


#endif //ZEUS_WORLDUNIFORMS_H
