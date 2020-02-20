//
// Created by aurailus on 24/09/19.
//

#pragma once

#include <glm/vec2.hpp>

#include "Shader.h"

class LightingShader : public Shader {
public:
    explicit LightingShader(glm::ivec2 windowSize, float bufferScale);
    void postCreate() override;

    void windowResized(glm::ivec2 windowSize);

    struct Uniforms {
        GLint gPosition;
        GLint gNormal;
        GLint gColorSpec;

        GLint camPosition;
    };

    Uniforms uniforms {};

    unsigned int gBuffer;
    unsigned int gPosition;
    unsigned int gNormal;
    unsigned int gColorSpec;
    unsigned int rDepth;

private:
    glm::ivec2 windowSize {};
    float bufferScale = 1;
};
