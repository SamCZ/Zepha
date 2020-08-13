//
// Created by aurailus on 24/09/19.
//

#include <glm/glm.hpp>

#include "SSAOShader.h"

SSAOShader::SSAOShader(glm::ivec2 windowSize, float bufferScale, unsigned int kernelCount) : Shader(),
    windowSize(windowSize),
    bufferScale(bufferScale),
    kernelCount(kernelCount) {}

float lerp(float a, float b, float f) {
    return a + f * (b - a);
}

void SSAOShader::postCreate() {
    uniforms.proj        = get("projection");
    uniforms.view        = get("view");
    uniforms.kernelCount = get("kernels");
    uniforms.sampleScale = get("sampleScale");

    use();
    set(uniforms.sampleScale, 2.0f);

    std::uniform_real_distribution<float> rand(0.0, 1.0);
    std::default_random_engine generator;

    for (unsigned int i = 0; i < kernelCount; i++) {
        glm::vec3 sample {
            rand(generator) * 2.0 - 1.0,
            rand(generator) * 2.0 - 1.0,
            rand(generator)
        };

        sample = glm::normalize(sample);
        sample *= rand(generator);

        float scale = static_cast<float>(i / kernelCount);
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;

        kernels.push_back(sample);
    }

    for (unsigned int i = 0; i < sampleCount; i++) {
        noise.emplace_back(rand(generator) * 2.0 - 1.0, rand(generator) * 2.0 - 1.0, 0.0f);
    }

    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &noise[0]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
        glGenTextures(1, &colorBuffer);
        glBindTexture(GL_TEXTURE_2D, colorBuffer);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<int>(windowSize.x * bufferScale), static_cast<int>(windowSize.y * bufferScale), 0, GL_RGB, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void SSAOShader::windowResized(glm::ivec2 windowSize) {
    this->windowSize = windowSize;

    glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<int>(windowSize.x * bufferScale), static_cast<int>(windowSize.y * bufferScale), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}
