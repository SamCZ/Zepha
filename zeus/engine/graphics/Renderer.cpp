//
// Created by aurailus on 17/12/18.
//

#include "Renderer.h"

Renderer::Renderer(GLint winWidth, GLint winHeight) {

    window = new Window(winWidth, winHeight);
    window->initialize();
    camera = new Camera(glm::vec3(0.0f, 16.0f, 0.0f), glm::vec3(0, 1, 0), -90.0f, -45.0f, 10.0f, 0.1f);

    shader = new Shader();
    shader->createFromFile("../zeus/shader/world.vs", "../zeus/shader/world.fs");

    projectionMatrix = glm::perspective(45.0f, window->getBufferWidth() / window->getBufferHeight(), 0.1f, 1000.0f);

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::update() {
    window->update();
}

void Renderer::begin() {
    //Clear Window
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader->useShader();

    glUniformMatrix4fv(shader->getProjectionLocation(), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->getViewLocation(), 1, GL_FALSE, glm::value_ptr(camera->calculateViewMatrix()));
}

void Renderer::draw(Entity* entity) {
    glUniformMatrix4fv(shader->getModelLocation(), 1, GL_FALSE, glm::value_ptr(entity->getModelMatrix()));
    entity->draw();
}

void Renderer::end() {
    Shader::clearShader();
    window->swapBuffers();
}

Renderer::~Renderer() {

}

Window *Renderer::getWindow() {
    return window;
}

Camera *Renderer::getCamera() {
    return camera;
}
