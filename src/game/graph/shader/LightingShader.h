//
// Created by aurailus on 24/09/19.
//

#pragma once

#include "Shader.h"

class LightingShader : public Shader {
public:
    explicit LightingShader(glm::vec2 windowSize, float bufferScale);
    void postCreate() override;

    void windowResized(glm::vec2 windowSize);

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
    glm::vec2 windowSize {};
    float bufferScale = 1;
};
