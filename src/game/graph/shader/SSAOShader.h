//
// Created by aurailus on 24/09/19.
//

#pragma once

#include "Shader.h"
#include <random>
typedef unsigned int uint;

class SSAOShader : public Shader {
public:
    explicit SSAOShader(glm::vec2 windowSize, float bufferScale, uint kernelCount);
    void postCreate() override;

    void windowResized(glm::vec2 windowSize);

    struct Uniforms {
        GLint proj;
        GLint view;
        GLint kernelCount;
        GLint sampleScale;
    };

    Uniforms uniforms {};

    uint kernelCount = 32;
    uint sampleCount = 16;

    std::vector<glm::vec3> kernels {};
    std::vector<glm::vec3> noise {};

    unsigned int tex;
    unsigned int fbo;
    unsigned int colorBuffer;

private:
    glm::vec2 windowSize {};
    float bufferScale = 1;
};
