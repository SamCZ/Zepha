//
// Created by aurailus on 17/12/18.
//

#include "Renderer.h"

Renderer::Renderer() : Renderer(1366, 768) {};

Renderer::Renderer(GLint winWidth, GLint winHeight) :
    activeTexture(nullptr),
    window(winWidth, winHeight),

    world({winWidth, winHeight}, 2),
    entity({winWidth, winHeight}, 2),
    ssao({winWidth, winHeight}, 1, 16),
    blur({winWidth, winHeight}, 1),
    lighting({winWidth, winHeight}, 2) {

    window.initialize();
    camera.create(window.getSize().x, window.getSize().y, glm::vec3(0, 1, 0));

    ssao.createFromFile("./assets/shader/post/passThrough.vs", "./assets/shader/post/ssaoCalc.fs");
    blur.createFromFile("./assets/shader/post/passThrough.vs", "./assets/shader/post/ssaoBlur.fs");
    lighting.createFromFile("./assets/shader/post/passThrough.vs", "./assets/shader/post/deferredLighting.fs");
    world.createFromFile("./assets/shader/world/deferredGeometryWorld.vs", "./assets/shader/world/deferredGeometryWorld.fs");
    entity.createFromFile("./assets/shader/world/deferredGeometryEntity.vs", "./assets/shader/world/deferredGeometryEntity.fs");

    guiShader = Shader();
    guiShader.createFromFile("./assets/shader/ortho/hud.vs", "./assets/shader/ortho/hud.fs");

    gu.matrix = camera.getOrthographicMatrix();
    gu.ortho  = guiShader.get("ortho");
    gu.model  = guiShader.get("model");

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
}

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

void Renderer::update(double delta) {
    //VSync 1 = On, 0 = Off
    glfwSwapInterval(1);
    elapsedTime += delta;

    window.update();
    world.updateSwayMap(delta);

    if (window.resized) {
        ssao.windowResized(window.getSize());
        blur.windowResized(window.getSize());
        lighting.windowResized(window.getSize());
        world.windowResized(window.getSize());

        resized = true;
        window.resized = false;
        camera.changeWindowDimensions(window.getSize());

        gu.matrix = camera.getOrthographicMatrix();
    }
}

void Renderer::beginChunkDeferredCalls() {
    activeTexture = nullptr;
    currentModelUniform = world.uniforms.model;

    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glViewport(0, 0, static_cast<int>(world.windowSize.x * world.bufferScale), static_cast<int>(world.windowSize.y * world.bufferScale));

    glBindFramebuffer(GL_FRAMEBUFFER, lighting.gBuffer);
    glClear(GL_DEPTH_BUFFER_BIT);

    const float skyColor[] = {clearColor.x, clearColor.y, clearColor.z, 1};
    static const float clearTransparent[] = {0, 0, 0, 1};
    glClearBufferfv(GL_COLOR, 0, clearTransparent);
    glClearBufferfv(GL_COLOR, 1, clearTransparent);
    glClearBufferfv(GL_COLOR, 2, skyColor);

    world.use();
    world.set(world.uniforms.proj, camera.getProjectionMatrix());
    world.set(world.uniforms.view, camera.getViewMatrix());
    world.set(world.uniforms.time, static_cast<float>(elapsedTime));
    world.swayTex.use(1);
}

void Renderer::beginEntityDeferredCalls() {
    currentModelUniform = entity.uniforms.model;

    entity.use();
    entity.set(entity.uniforms.proj, camera.getProjectionMatrix());
    entity.set(entity.uniforms.view, camera.getViewMatrix());
}

void Renderer::endDeferredCalls() {
    activeTexture = nullptr;

    glBindFramebuffer(GL_FRAMEBUFFER, ssao.fbo);
    glClearColor(clearColor.x, clearColor.y, clearColor.z, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    ssao.use();
    ssao.set(ssao.uniforms.proj, camera.getProjectionMatrix());
    ssao.set(ssao.uniforms.view, camera.getViewMatrix());
    ssao.set(ssao.uniforms.kernelCount, ssao.kernelCount);

    for (unsigned int i = 0; i < ssao.kernelCount; i++) {
        GLint uni = ssao.get("samples[" + std::to_string(i) + "]");
        ssao.set(uni, ssao.kernels[i]);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, lighting.gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, lighting.gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, ssao.tex);

    renderQuad();

    auto winSize = window.getSize();
    glViewport(0, 0, static_cast<int>(winSize.x), static_cast<int>(winSize.y));

    glBindFramebuffer(GL_FRAMEBUFFER, blur.fbo);
    glClear(GL_COLOR_BUFFER_BIT);
    blur.use();
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, ssao.colorBuffer);
    renderQuad();

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lighting.use();
    lighting.set(lighting.uniforms.camPosition, camera.getPos());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, blur.colorBuffer);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, lighting.gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, lighting.gColorSpec);

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

void Renderer::setClearColor(unsigned char r, unsigned char g, unsigned char b) {
    clearColor = {static_cast<float>(r)/255.f, static_cast<float>(g)/255.f, static_cast<float>(b)/255.f, 1};
}

void Renderer::setBones(std::vector<glm::mat4> &transforms) {
    if (transforms.empty()) return;
    entity.setArr(entity.uniforms.bones, static_cast<GLsizei>(transforms.size()), transforms.at(0));
}
