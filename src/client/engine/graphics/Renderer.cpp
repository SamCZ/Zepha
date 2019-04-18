//
// Created by aurailus on 17/12/18.
//

#include "Renderer.h"

Renderer::Renderer() : Renderer(1366, 768) {};

Renderer::Renderer(GLint winWidth, GLint winHeight) {
    mode = false;
    activeTexture = nullptr;

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

    //Get GUI shader uniforms
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
    glClearColor(0.58f, 0.76f, 0.94f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    enableWorldShader();
}

void Renderer::beginGUI() {
    glClear(GL_DEPTH_BUFFER_BIT);
    enableGuiShader();
}

void Renderer::end() {
    Shader::clearShader();
    window->swapBuffers();
}

Window *Renderer::getWindow() {
    return window;
}

Camera *Renderer::getCamera() {
    return camera;
}

void Renderer::enableWorldShader() {
    worldShader->useShader();
    mode = false;

    glUniformMatrix4fv(uProj, 1, GL_FALSE, glm::value_ptr(*projectionMatrix));
    glUniformMatrix4fv(uView, 1, GL_FALSE, glm::value_ptr(camera->getViewMatrix()));
}

void Renderer::enableGuiShader() {
    guiShader->useShader();
    mode = true;

    glUniformMatrix4fv(uOrtho, 1, GL_FALSE, glm::value_ptr(*orthographicMatrix));
}

void Renderer::setModelMatrix(glm::mat4& modelMatrix) {
    glUniformMatrix4fv((mode) ? uGuiModel : uModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
}

void Renderer::enableTexture(Texture *texture) {
    if (texture != activeTexture) {
        activeTexture = texture;
        texture->enable();
    }
}

Renderer::~Renderer() {
    worldShader->cleanup();
    delete worldShader;

    guiShader->cleanup();
    delete guiShader;
}