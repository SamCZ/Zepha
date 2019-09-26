//
// Created by aurailus on 25/09/19.
//

#pragma once

#include "Shader.h"

class EntityGeometryShader : public Shader {
public:
    explicit EntityGeometryShader(glm::vec2 windowSize, float bufferScale);
    void postCreate() override;

    struct Uniforms {
        GLint proj;
        GLint model;
        GLint view;

        GLint bones;
    };

    Uniforms uniforms {};

private:
    glm::vec2 windowSize {};
    float bufferScale = 1;
};
