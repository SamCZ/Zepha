//
// Created by aurailus on 06/01/19.
//

#include "Client.h"

Client::Client() = default;

Client::Client(int width, int height) {
    renderer = new Renderer(width, height);
}

void Client::start() {
    scene = new GameInstance();
    scene->initialize(renderer);

    while (!renderer->getWindow()->getShouldClose()) loop();
}

void Client::loop() {
    Timer t("Game Loop");

    double now = glfwGetTime();
    double delta = now - timeElapsed;
    timeElapsed = now;

    glfwPollEvents();

    scene->update(delta, fps);
    scene->draw();

    fps = 1000 / (t.elapsedNs() / 1000000.0);
}

void Client::cleanup() {
    delete scene;
    delete renderer;
}

Client::~Client() {
    cleanup();
}