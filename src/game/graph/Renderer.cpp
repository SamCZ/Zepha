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
    camera.create(window.getBufferWidth(), window.getBufferHeight(), glm::vec3(0, 1, 0));

    worldShader = Shader();
    worldShader.createFromFile("../res/shader/world.vs", "../res/shader/world.fs");

    wu.matrix = camera.getProjectionMatrix();
    wu.proj   = worldShader.getUniform("projection");
    wu.model  = worldShader.getUniform("model");
    wu.view   = worldShader.getUniform("view");

    guiShader = Shader();
    guiShader.createFromFile("../res/shader/gui.vs", "../res/shader/gui.fs");

    gu.matrix = camera.getOrthographicMatrix();
    gu.ortho  = guiShader.getUniform("ortho");
    gu.model  = guiShader.getUniform("model");

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::update() {
    window.update();

    if (window.resized) {
        resized = true;
        camera.changeWindowDimensions(window.getBufferWidth(), window.getBufferHeight());
        window.resized = false;
        wu.matrix = camera.getProjectionMatrix();
        gu.matrix = camera.getOrthographicMatrix();
    }
}

void Renderer::begin() {
    glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);

    enableWorldShader();
}

void Renderer::beginGUI() {
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_DEPTH_TEST);
    enableGuiShader();
}

void Renderer::end() {
    Shader::clearShader();
    window.swapBuffers();
}

Window *Renderer::getWindow() {
    return &window;
}

Camera *Renderer::getCamera() {
    return &camera;
}

void Renderer::enableWorldShader() {
    worldShader.useShader();
    mode = false;

    glUniformMatrix4fv(wu.proj, 1, GL_FALSE, glm::value_ptr(wu.matrix));
    glUniformMatrix4fv(wu.view, 1, GL_FALSE, glm::value_ptr(camera.getViewMatrix()));
}

void Renderer::enableGuiShader() {
    guiShader.useShader();
    mode = true;

    glUniformMatrix4fv(gu.ortho, 1, GL_FALSE, glm::value_ptr(gu.matrix));
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
    worldShader.cleanup();
    guiShader.cleanup();
}