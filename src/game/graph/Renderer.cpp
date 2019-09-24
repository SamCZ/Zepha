//
// Created by aurailus on 17/12/18.
//

#include <random>
#include "Renderer.h"

Renderer::Renderer() : Renderer(1366, 768) {};

float lerp(float a, float b, float f) {
    return a + f * (b - a);
}

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

    swayNoise.SetFrequency(0.5);
    swayNoise.SetOctaveCount(2);
    swayTex.loadFromBytes(swayData, 16, 16, GL_LINEAR, GL_MIRRORED_REPEAT);

    std::uniform_real_distribution<float> randomFloats(0.0, 1.0); // random floats between 0.0 - 1.0
    std::default_random_engine generator;
    for (unsigned int i = 0; i < samples; i++) {
        glm::vec3 sample(
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator) * 2.0 - 1.0,
                randomFloats(generator)
        );
        sample = glm::normalize(sample);
        sample *= randomFloats(generator);
        float scale = static_cast<float>(i / 64.0);
        scale = lerp(0.1f, 1.0f, scale * scale);
        sample *= scale;
        ssaoKernel.push_back(sample);
    }

    for (unsigned int i = 0; i < 16; i++) {
        ssaoNoise.emplace_back(randomFloats(generator) * 2.0 - 1.0,
                               randomFloats(generator) * 2.0 - 1.0,
                               0.0f);
    }

    ssaoShader = Shader();
    ssaoShader.createFromFile("./assets/shader/post/passThrough.vs", "./assets/shader/post/ssaoCalc.fs");

    sau.proj = ssaoShader.get("projection");
    sau.view = ssaoShader.get("view");
    sau.kernels = ssaoShader.get("kernels");

    glGenTextures(1, &ssaoTex);
    glBindTexture(GL_TEXTURE_2D, ssaoTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glGenFramebuffers(1, &ssaoFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);

    glGenTextures(1, &ssaoColorBuffer);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, winWidth / 2, winHeight / 2, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBuffer, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    sau.matrix = camera.getProjectionMatrix();

    ssaoBlur = Shader();
    ssaoBlur.createFromFile("./assets/shader/post/passThrough.vs", "./assets/shader/post/ssaoBlur.fs");

    glGenFramebuffers(1, &ssaoBlurFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glGenTextures(1, &ssaoColorBufferBlur);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, winWidth, winHeight, 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, ssaoColorBufferBlur, 0);

    createWorldShaders();
    createGUIShader();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
}

void Renderer::createWorldShaders() {
    //Initialize World Geometry Shader

    worldGeometryShader = Shader();
    worldGeometryShader.createFromFile("./assets/shader/world/deferredGeometryWorld.vs", "./assets/shader/world/deferredGeometryWorld.fs");

    wgu.matrix = camera.getProjectionMatrix();
    wgu.proj   = worldGeometryShader.get("projection");
    wgu.model  = worldGeometryShader.get("model");
    wgu.view   = worldGeometryShader.get("view");

    wgu.swaySampler = worldGeometryShader.get("swayTex");

    wgu.time   = worldGeometryShader.get("time");

    worldGeometryShader.use();
    worldGeometryShader.set(wgu.swaySampler, 1);

    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    auto winSize = window.getSize();

    winSize *= renderScale;

    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<int>(winSize.x), static_cast<int>(winSize.y), 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
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
    entityGeometryShader.createFromFile("./assets/shader/world/deferredGeometryEntity.vs", "./assets/shader/world/deferredGeometryEntity.fs");

    egu.matrix = camera.getProjectionMatrix();
    egu.proj   = entityGeometryShader.get("projection");
    egu.model  = entityGeometryShader.get("model");
    egu.view   = entityGeometryShader.get("view");

    egu.uBones = entityGeometryShader.get("uBones");

    //Initialize Lighting Shader for Deferred Rendering

    worldLightingShader = Shader();
    worldLightingShader.createFromFile("./assets/shader/post/passThrough.vs", "./assets/shader/post/deferredLighting.fs");

    wlu.gPosition  = worldLightingShader.get("gPosition");
    wlu.gNormal    = worldLightingShader.get("gNormal");
    wlu.gColorSpec = worldLightingShader.get("gColorSpec");

    wlu.camPosition = worldLightingShader.get("camPosition");

    worldLightingShader.use();
    worldLightingShader.set(wlu.gPosition, 0);
    worldLightingShader.set(wlu.gNormal, 1);
    worldLightingShader.set(wlu.gColorSpec, 2);

    //Initialize Shading Shader for Shadowmapping

//    const unsigned int SHADOW_SCALE = 2048;
//    glGenFramebuffers(1, &sBuffer);
//    glGenTextures(1, &sDepthMap);
//    glBindTexture(GL_TEXTURE_2D, sDepthMap);
//    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_SCALE, SHADOW_SCALE, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//    glBindFramebuffer(GL_FRAMEBUFFER, sBuffer);
//    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, sDepthMap, 0);
//    glDrawBuffer(GL_NONE);
//    glReadBuffer(GL_NONE);
//    glBindBuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::createGUIShader() {
    guiShader = Shader();
    guiShader.createFromFile("./assets/shader/ortho/hud.vs", "./assets/shader/ortho/hud.fs");

    gu.matrix = camera.getOrthographicMatrix();
    gu.ortho  = guiShader.get("ortho");
    gu.model  = guiShader.get("model");
}

void Renderer::update(double delta) {
    //VSync 1 = On, 0 = Off
    glfwSwapInterval(1);

    window.update();
    elapsedTime += delta;

    swayOffset += delta * 1.4;
    for (int i = 0; i < 16 * 16; i++) {
        swayData[i*4]   = static_cast<unsigned char>((fmax(-1, fmin(1, swayNoise.GetValue((i / 16) / 3.f, (i % 16) / 3.f, swayOffset)))       + 1) / 2.f * 255.f);
        swayData[i*4+1] = static_cast<unsigned char>((fmax(-1, fmin(1, swayNoise.GetValue((i / 16) / 3.f, (i % 16) / 3.f, swayOffset + 50)))  + 1) / 2.f * 255.f);
        swayData[i*4+2] = static_cast<unsigned char>((fmax(-1, fmin(1, swayNoise.GetValue((i / 16) / 3.f, (i % 16) / 3.f, swayOffset + 100))) + 1) / 2.f * 255.f);
    }
    swayTex.updateTexture(0, 0, 16, 16, swayData);

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

        glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<int>(winSize.x / 2), static_cast<int>(winSize.y / 2), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

        glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, static_cast<int>(winSize.x / 2), static_cast<int>(winSize.y / 2), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

        wgu.matrix = camera.getProjectionMatrix();
        egu.matrix = camera.getProjectionMatrix();
        sau.matrix = camera.getProjectionMatrix();
        gu.matrix = camera.getOrthographicMatrix();
    }
}

void Renderer::beginChunkDeferredCalls() {
    activeTexture = nullptr;
    currentModelUniform = wgu.model;

    auto winSize = window.getSize();
    winSize *= renderScale;

    glClearColor(clearColor.x, clearColor.y, clearColor.z, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);
    glViewport(0, 0, static_cast<int>(winSize.x), static_cast<int>(winSize.y));

    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    worldGeometryShader.use();

    worldGeometryShader.set(wgu.proj, wgu.matrix);
    worldGeometryShader.set(wgu.view, camera.getViewMatrix());

    worldGeometryShader.set(wgu.time, static_cast<float>(elapsedTime));

    swayTex.use(1);
}

void Renderer::beginEntityDeferredCalls() {
    currentModelUniform = egu.model;

    entityGeometryShader.use();

    entityGeometryShader.set(egu.proj, egu.matrix);
    entityGeometryShader.set(egu.view, camera.getViewMatrix());
}

void Renderer::endDeferredCalls() {
    activeTexture = nullptr;

    glBindFramebuffer(GL_FRAMEBUFFER, ssaoFBO);
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    ssaoShader.use();
    ssaoShader.set(sau.proj, sau.matrix);
    ssaoShader.set(sau.view, camera.getViewMatrix());
    ssaoShader.set(sau.kernels, samples);

    for (unsigned int i = 0; i < samples; i++) {
        ssaoShader.set(ssaoShader.get("samples[" + std::to_string(i) + "]"), ssaoKernel[i]);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, ssaoTex);

    renderQuad();

    auto winSize = window.getSize();
    glViewport(0, 0, static_cast<int>(winSize.x), static_cast<int>(winSize.y));

    glBindFramebuffer(GL_FRAMEBUFFER, ssaoBlurFBO);
    glClear(GL_COLOR_BUFFER_BIT);
    ssaoBlur.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
    renderQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    worldLightingShader.use();

    worldLightingShader.set(wlu.camPosition, camera.getPos());

//    glActiveTexture(GL_TEXTURE0);
//    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ssaoColorBufferBlur);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gColorSpec);

    glEnable(GL_BLEND);
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
    activeTexture = nullptr;
    currentModelUniform = gu.model;

    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);

    guiShader.use();
    guiShader.set(gu.ortho, gu.matrix);
}

void Renderer::swapBuffers() {
    Shader::clearShader();
    window.swapBuffers();
}

Window& Renderer::getWindow() {
    return window;
}

Camera& Renderer::getCamera() {
    return camera;
}

void Renderer::renderQuad() {
    if (quadVAO == 0) {
        float quadVertices[] = {
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
             1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
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
        texture->use(0);
    }
}

Renderer::~Renderer() {
    worldGeometryShader.cleanup();
    guiShader.cleanup();
    swayTex.clear();
    delete[] swayData;
}

void Renderer::setClearColor(unsigned char r, unsigned char g, unsigned char b) {
    clearColor = {static_cast<float>(r)/255.f, static_cast<float>(g)/255.f, static_cast<float>(b)/255.f, 1};
}

void Renderer::setBones(std::vector<glm::mat4> &transforms) {
    if (transforms.empty()) return;
    entityGeometryShader.setArr(egu.uBones, static_cast<GLsizei>(transforms.size()), transforms.at(0));
}
