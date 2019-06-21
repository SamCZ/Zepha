//
// Created by aurailus on 17/12/18.
//

#include "Renderer.h"

Renderer::Renderer() : Renderer(1366, 768) {};

Renderer::Renderer(GLint winWidth, GLint winHeight) :
    activeTexture(nullptr),

    window(winWidth, winHeight),
    swayData(new unsigned char[16 * 4 * 16]) {

    window.initialize();
    auto winSize = window.getSize();
    camera.create(winSize.x, winSize.y, glm::vec3(0, 1, 0));

    for (int i = 0; i < 16 * 16; i++) {
        swayData[i*4+1] = 127;
        swayData[i*4+3] = 255;
    }

    swayNoise.SetOctaveCount(2);

    swayMap = new Texture();
    swayMap->loadFromBytes(swayData, 16, 16, GL_LINEAR, GL_MIRRORED_REPEAT);

    createWorldShaders();
    createGUIShader();

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::createWorldShaders() {
    //Initialize World Geometry Shader

    worldGeometryShader = Shader();
    worldGeometryShader.createFromFile("../res/shader/world/deferredGeometryWorld.vs", "../res/shader/world/deferredGeometryWorld.fs");

    wgu.matrix = camera.getProjectionMatrix();
    wgu.proj   = worldGeometryShader.getUniform("projection");
    wgu.model  = worldGeometryShader.getUniform("model");
    wgu.view   = worldGeometryShader.getUniform("view");

    wgu.time   = worldGeometryShader.getUniform("time");

    worldGeometryShader.use();

    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    auto winSize = window.getSize();

    winSize *= renderScale;

    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<int>(winSize.x), static_cast<int>(winSize.y), 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<int>(winSize.x), static_cast<int>(winSize.y), 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    glGenTextures(1, &gColorSpec);
    glBindTexture(GL_TEXTURE_2D, gColorSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(winSize.x), static_cast<int>(winSize.y), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

    unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<int>(winSize.x), static_cast<int>(winSize.y));
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //Initialize Entity Geometry Shader

    entityGeometryShader = Shader();
    entityGeometryShader.createFromFile("../res/shader/world/deferredGeometryEntity.vs", "../res/shader/world/deferredGeometryEntity.fs");

    egu.matrix = camera.getProjectionMatrix();
    egu.proj   = entityGeometryShader.getUniform("projection");
    egu.model  = entityGeometryShader.getUniform("model");
    egu.view   = entityGeometryShader.getUniform("view");

    //Initialize Lighting Shader for Deferred Rendering

    worldLightingShader = Shader();
    worldLightingShader.createFromFile("../res/shader/world/deferredLighting.vs", "../res/shader/world/deferredLighting.fs");

    wlu.gPosition  = worldLightingShader.getUniform("gPosition");
    wlu.gNormal    = worldLightingShader.getUniform("gNormal");
    wlu.gColorSpec = worldLightingShader.getUniform("gColorSpec");

    wlu.camPosition = worldLightingShader.getUniform("camPosition");

    worldLightingShader.use();
    glUniform1i(wlu.gPosition, 0);
    glUniform1i(wlu.gNormal, 1);
    glUniform1i(wlu.gColorSpec, 2);

    //Initialize Shading Shader for Shadowmapping

    const unsigned int SHADOW_SCALE = 2048;
    glGenFramebuffers(1, &sBuffer);
    glGenTextures(1, &sDepthMap);
    glBindTexture(GL_TEXTURE_2D, sDepthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_SCALE, SHADOW_SCALE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glBindFramebuffer(GL_FRAMEBUFFER, sBuffer);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sDepthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindBuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::createGUIShader() {
    guiShader = Shader();
    guiShader.createFromFile("../res/shader/ortho/hud.vs", "../res/shader/ortho/hud.fs");

    gu.matrix = camera.getOrthographicMatrix();
    gu.ortho  = guiShader.getUniform("ortho");
    gu.model  = guiShader.getUniform("model");
}

void Renderer::update(double delta) {
    window.update();
    elapsedTime += delta;

    swayOffset += delta * 0.9;
    for (int i = 0; i < 16 * 16; i++) {
        swayData[i*4]   = static_cast<unsigned char>((fmax(-1, fmin(1, swayNoise.GetValue((i / 16) / 3.f, (i % 16) / 3.f, swayOffset)))       + 1) / 2.f * 255.f);
        swayData[i*4+1] = static_cast<unsigned char>((fmax(-1, fmin(1, swayNoise.GetValue((i / 16) / 3.f, (i % 16) / 3.f, swayOffset + 50)))  + 1) / 2.f * 255.f);
        swayData[i*4+2] = static_cast<unsigned char>((fmax(-1, fmin(1, swayNoise.GetValue((i / 16) / 3.f, (i % 16) / 3.f, swayOffset + 100))) + 1) / 2.f * 255.f);
    }
    swayMap->updateTexture(0, 0, 16, 16, swayData);

    if (window.resized) {
        resized = true;
        window.resized = false;

        auto winSize = window.getSize();
        camera.changeWindowDimensions(winSize.x, winSize.y);

        winSize *= renderScale;

        glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

        glBindTexture(GL_TEXTURE_2D, gPosition);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<int>(winSize.x), static_cast<int>(winSize.y), 0, GL_RGB, GL_FLOAT, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

        glBindTexture(GL_TEXTURE_2D, gNormal);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<int>(winSize.x), static_cast<int>(winSize.y), 0, GL_RGB, GL_FLOAT, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

        glBindTexture(GL_TEXTURE_2D, gColorSpec);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(winSize.x), static_cast<int>(winSize.y), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

        unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
        glDrawBuffers(3, attachments);

        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<int>(winSize.x), static_cast<int>(winSize.y));
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
            std::cout << "Framebuffer not complete!" << std::endl;
        }
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        wgu.matrix = camera.getProjectionMatrix();
        egu.matrix = camera.getProjectionMatrix();
        gu.matrix = camera.getOrthographicMatrix();
    }
}

void Renderer::beginChunkDeferredCalls() {
    activeTexture = nullptr;
    currentModelUniform = wgu.model;

    auto winSize = window.getSize();
    winSize *= renderScale;

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, static_cast<int>(winSize.x), static_cast<int>(winSize.y));

    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    swayMap->enable(1);

    worldGeometryShader.use();
    glUniformMatrix4fv(wgu.proj, 1, GL_FALSE, glm::value_ptr(wgu.matrix));
    glUniformMatrix4fv(wgu.view, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));

    glUniform1f(wgu.time, static_cast<float>(elapsedTime));
}

void Renderer::beginEntityDeferredCalls() {
    currentModelUniform = egu.model;

    entityGeometryShader.use();
    glUniformMatrix4fv(egu.proj, 1, GL_FALSE, glm::value_ptr(egu.matrix));
    glUniformMatrix4fv(egu.view, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
}

void Renderer::endDeferredCalls() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    auto winSize = window.getSize();
    glViewport(0, 0, static_cast<int>(winSize.x), static_cast<int>(winSize.y));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

    worldLightingShader.use();

    glUniform3f(wlu.camPosition, camera.getPos().x, camera.getPos().y, camera.getPos().z);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gColorSpec);

    renderQuad();

    //Used to push the depth map to the default framebuffer
    //Enable when creating forward rendering stage

//    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
//    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
//
//    auto winSize = window.getSize();
//    glBlitFramebuffer(0, 0, static_cast<int>(winSize.x), static_cast<int>(winSize.y),
//                      0, 0, static_cast<int>(winSize.x), static_cast<int>(winSize.y),
//                      GL_DEPTH_BUFFER_BIT, GL_NEAREST);
//    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::beginGUIDrawCalls() {
    currentModelUniform = gu.model;

    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    guiShader.use();
    glUniformMatrix4fv(gu.ortho, 1, GL_FALSE, glm::value_ptr(gu.matrix));
}

void Renderer::swapBuffers() {
    Shader::clearShader();
    window.swapBuffers();
}

Window *Renderer::getWindow() {
    return &window;
}

Camera *Renderer::getCamera() {
    return &camera;
}

void Renderer::renderQuad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
            -1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) nullptr);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    }

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}

void Renderer::setModelMatrix(const glm::mat4& modelMatrix) {
    glUniformMatrix4fv(currentModelUniform, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void Renderer::enableTexture(Texture *texture) {
    if (texture != activeTexture) {
        activeTexture = texture;
        texture->enable();
    }
}

Renderer::~Renderer() {
    worldGeometryShader.cleanup();
    guiShader.cleanup();
}