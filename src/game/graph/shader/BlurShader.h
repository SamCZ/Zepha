//
// Created by aurailus on 24/09/19.
//

#pragma once

#include "Shader.h"

class BlurShader : public Shader {
public:
    explicit BlurShader(glm::vec2 windowSize, float bufferScale);
    void postCreate() override;

    void windowResized(glm::vec2 windowSize);

    struct Uniforms {
        GLint sampleScale;
    };

    Uniforms uniforms {};

    unsigned int fbo;
    unsigned int colorBuffer;

private:
    glm::vec2 windowSize;
    float bufferScale;
};
