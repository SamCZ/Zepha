//
// Created by aurailus on 24/09/19.
//

#include "BlurShader.h"

BlurShader::BlurShader(glm::vec2 windowSize, float bufferScale) : Shader(),
   windowSize(windowSize),
   bufferScale(bufferScale) {}

void BlurShader::postCreate() {
    uniforms.sampleScale = get("sampleScale");

    use();
    set(uniforms.sampleScale, 1.f);

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

void BlurShader::windowResized(glm::vec2 windowSize) {
    glBindTexture(GL_TEXTURE_2D, colorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<int>(windowSize.x * bufferScale), static_cast<int>(windowSize.y * bufferScale), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
}