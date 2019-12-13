//
// Created by aurailus on 24/09/19.
//

#pragma once

#include "Shader.h"
typedef unsigned int uint;

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

    uint gBuffer;
    uint gPosition;
    uint gNormal;
    uint gColorSpec;
    uint rDepth;

private:
    glm::ivec2 windowSize {};
    float bufferScale = 1;
};
