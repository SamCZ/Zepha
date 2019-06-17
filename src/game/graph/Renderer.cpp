//
// Created by aurailus on 17/12/18.
//

#include "Renderer.h"

Renderer::Renderer() : Renderer(1366, 768) {};

Renderer::Renderer(GLint winWidth, GLint winHeight) :
    mode(false),
    activeTexture(nullptr),

    window(winWidth, winHeight) {

    window.initialize();
    auto winSize = window.getSize();
    camera.create(winSize.x, winSize.y, glm::vec3(0, 1, 0));

    createWorldShader();
    createGUIShader();

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::createWorldShader() {
    worldGeometryShader = Shader();
    worldGeometryShader.createFromFile("../res/shader/geometryVertex.vs", "../res/shader/geometryFragment.fs");

    worldLightingShader = Shader();
    worldLightingShader.createFromFile("../res/shader/lightingVertex.vs", "../res/shader/lightingFragment.fs");

    wu.matrix = camera.getProjectionMatrix();
    wu.proj   = worldGeometryShader.getUniform("projection");
    wu.model  = worldGeometryShader.getUniform("model");
    wu.view   = worldGeometryShader.getUniform("view");

    worldGeometryShader.use();

    glGenFramebuffers(1, &gBuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

    auto winSize = window.getSize();

    glGenTextures(1, &gPosition);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<int>(winSize.x), static_cast<int>(winSize.y), 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);

    glGenTextures(1, &gNormal);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, static_cast<int>(winSize.x), static_cast<int>(winSize.y), 0, GL_RGB, GL_FLOAT, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

    glGenTextures(1, &gColorSpec);
    glBindTexture(GL_TEXTURE_2D, gColorSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(winSize.x), static_cast<int>(winSize.y), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gColorSpec, 0);

    unsigned int attachments[3] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, attachments);

    unsigned int rboDepth;
    glGenRenderbuffers(1, &rboDepth);
    glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, static_cast<int>(winSize.x), static_cast<int>(winSize.y));
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        std::cout << "Framebuffer not complete!" << std::endl;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    wu.gPosition  = worldLightingShader.getUniform("gPosition");
    wu.gNormal    = worldLightingShader.getUniform("gNormal");
    wu.gColorSpec = worldLightingShader.getUniform("gColorSpec");

    worldLightingShader.use();
    glUniform1i(wu.gPosition, 0);
    glUniform1i(wu.gNormal, 1);
    glUniform1i(wu.gColorSpec, 2);
}

void Renderer::createGUIShader() {
    guiShader = Shader();
    guiShader.createFromFile("../res/shader/gui.vs", "../res/shader/gui.fs");

    gu.matrix = camera.getOrthographicMatrix();
    gu.ortho  = guiShader.getUniform("ortho");
    gu.model  = guiShader.getUniform("model");
}

void Renderer::update() {
    window.update();

    if (window.resized) {
        resized = true;
        window.resized = false;

        auto winSize = window.getSize();
        camera.changeWindowDimensions(winSize.x, winSize.y);

        wu.matrix = camera.getProjectionMatrix();
        gu.matrix = camera.getOrthographicMatrix();
    }
}

void Renderer::beginWorldDrawCalls() {
    activeTexture = nullptr;

    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    worldGeometryShader.use();
    glUniformMatrix4fv(wu.proj, 1, GL_FALSE, glm::value_ptr(wu.matrix));
    glUniformMatrix4fv(wu.view, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
}

void Renderer::endWorldDrawCalls() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    worldLightingShader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gColorSpec);

    renderQuad();

    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    auto winSize = window.getSize();
    glBlitFramebuffer(0, 0, static_cast<int>(winSize.x), static_cast<int>(winSize.y),
                      0, 0, static_cast<int>(winSize.x), static_cast<int>(winSize.y),
                      GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::beginGUIDrawCalls() {
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
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *) (3 * sizeof(float)));
    }

    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}

void Renderer::setModelMatrix(glm::mat4& modelMatrix) {
    glUniformMatrix4fv((mode) ? gu.model : wu.model, 1, GL_FALSE, glm::value_ptr(modelMatrix));
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