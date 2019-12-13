//
// Created by aurailus on 24/09/19.
//

#include "LightingShader.h"

LightingShader::LightingShader(glm::ivec2 windowSize, float bufferScale) : Shader(),
    windowSize(windowSize),
    bufferScale(bufferScale) {}

void LightingShader::postCreate() {
    uniforms.gPosition   = get("gPosition");
    uniforms.gNormal     = get("gNormal");
    uniforms.gColorSpec  = get("gColorSpec");
    uniforms.camPosition = get("camPosition");

    use();
    set(uniforms.gPosition, 0);
    set(uniforms.gNormal, 1);
    set(uniforms.gColorSpec, 2);

    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

        glGenTextures(1, &gPosition);
        glBindTexture(GL_TEXTURE_2D, gPosition);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<int>(windowSize.x * bufferScale), static_cast<int>(windowSize.y * bufferScale), 0, GL_RGB, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

        glGenTextures(1, &gNormal);
        glBindTexture(GL_TEXTURE_2D, gNormal);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<int>(windowSize.x * bufferScale), static_cast<int>(windowSize.y * bufferScale), 0, GL_RGB, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

        glGenTextures(1, &gColorSpec);
        glBindTexture(GL_TEXTURE_2D, gColorSpec);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(windowSize.x), static_cast<int>(windowSize.y), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

        unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glDrawBuffers(3, attachments);

        glGenRenderbuffers(1, &rDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rDepth);

            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<int>(windowSize.x * bufferScale), static_cast<int>(windowSize.y * bufferScale));
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rDepth);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << Log::err << "-- Lighting framebuffer incomplete! --" << std::endl;
        }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void LightingShader::windowResized(glm::ivec2 windowSize) {
    this->windowSize = windowSize;

    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

        glBindTexture(GL_TEXTURE_2D, gPosition);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<int>(windowSize.x * bufferScale), static_cast<int>(windowSize.y * bufferScale), 0, GL_RGB, GL_FLOAT, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

        glBindTexture(GL_TEXTURE_2D, gNormal);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<int>(windowSize.x * bufferScale), static_cast<int>(windowSize.y * bufferScale), 0, GL_RGB, GL_FLOAT, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

        glBindTexture(GL_TEXTURE_2D, gColorSpec);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(windowSize.x * bufferScale), static_cast<int>(windowSize.y * bufferScale), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

        unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glDrawBuffers(3, attachments);

        glBindRenderbuffer(GL_RENDERBUFFER, rDepth);
            glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<int>(windowSize.x * bufferScale), static_cast<int>(windowSize.y * bufferScale));
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rDepth);

        glBindRenderbuffer(GL_RENDERBUFFER, 0);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << Log::err << "-- Lighting framebuffer incomplete! --" << std::endl;
        }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
