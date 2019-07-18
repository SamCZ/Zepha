//
// Created by aurailus on 16/06/19.
//

#ifndef ZEUS_GUIUNIFORMS_H
#define ZEUS_GUIUNIFORMS_H


#include <GL/gl.h>
#include <glm/mat4x4.hpp>

struct GuiUniforms {
    glm::mat4 matrix;

    GLint ortho;
    GLint model;
};

#endif //ZEUS_GUIUNIFORMS_H
