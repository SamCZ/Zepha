//
// Created by aurailus on 24/09/19.
//

#pragma once

#include <glm/vec2.hpp>

#include "Shader.h"

class BlurShader : public Shader {
public:
    explicit BlurShader(glm::ivec2 windowSize, float bufferScale);
    void postCreate() override;

    void windowResized(glm::ivec2 windowSize);

    struct Uniforms {
        GLint sampleScale;
    };

    Uniforms uniforms {};

    unsigned int fbo = 0;
    unsigned int colorBuffer = 0;

private:
    glm::ivec2 windowSize {};
    float bufferScale = 1;
};
