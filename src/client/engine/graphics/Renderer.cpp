//
// Created by aurailus on 17/12/18.
//

#include "Renderer.h"

Renderer::Renderer() : Renderer(1366, 768) {};

Renderer::Renderer(GLint winWidth, GLint winHeight) {
    //Create window
    window = new Window(winWidth, winHeight);
    window->initialize();

    //Create camera
    camera = new Camera();
    camera->create(window->getBufferWidth(), window->getBufferHeight(), glm::vec3(0, 1, 0));

    //Assign matrices
    projectionMatrix = camera->getProjectionMatrix();
    orthographicMatrix = camera->getOrthographicMatrix();

    //Initialize world shader
    worldShader = new Shader();
    worldShader->createFromFile("../res/shader/world.vs", "../res/shader/world.fs");

    //Get world shader uniforms
    uProj = worldShader->getUniformLocation("projection");
    uModel = worldShader->getUniformLocation("model");
    uView = worldShader->getUniformLocation("view");

    //Initialize GUI shader
    guiShader = new Shader();
    guiShader->createFromFile("../res/shader/gui.vs", "../res/shader/gui.fs");

    //Get gui shader uniforms
    uOrtho = guiShader->getUniformLocation("ortho");
    uGuiModel = guiShader->getUniformLocation("model");

    glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::update() {
    window->update();
    if (window->resized) {
        resized = true;
        camera->changeWindowDimensions(window->getBufferWidth(), window->getBufferHeight());
        window->resized = false;
    }
}

void Renderer::begin() {
    //Clear Window
    glClearColor(0.5f, 0.7f, 0.9f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::enableWorldShader() {
    worldShader->useShader();

    glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(*projectionMatrix));
    glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
}

void Renderer::enableGuiShader() {
    guiShader->useShader();

    glUniformMatrix4fv(uOrtho, 1, GL_FALSE, glm::value_ptr(*orthographicMatrix));
}

void Renderer::draw(Entity* entity) {
    glUniformMatrix4fv(uModel, 1, GL_FALSE, glm::value_ptr(entity->getModelMatrix()));
    entity->draw();
}

void Renderer::drawGui(Entity* entity) {
    glUniformMatrix4fv(uGuiModel, 1, GL_FALSE, glm::value_ptr(entity->getModelMatrix()));
    entity->draw();
}

void Renderer::end() {
    Shader::clearShader();
    window->swapBuffers();
}

Renderer::~Renderer() {
    worldShader->cleanup();
    delete worldShader;
    guiShader->cleanup();
    delete guiShader;
}

Window *Renderer::getWindow() {
    return window;
}

Camera *Renderer::getCamera() {
    return camera;
}