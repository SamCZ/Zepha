//
// Created by aurailus on 06/01/19.
//

#include "Client.h"
#include "../game/GameScene.h"

Client::Client() = default;

Client::Client(int width, int height) {
    renderer = new Renderer(width, height);
}

void Client::start() {
    state = new ClientState {
            .renderer = renderer,
            .fps = 0,
            .deltaTime = 0
    };

    Scene* s = new GameScene(state);
    sceneManager.setScene(s); //Main Menu Scene here eventually

    while (!renderer->getWindow()->getShouldClose()) loop();
}

void Client::loop() {
    Timer t("Client Loop");

    double now = glfwGetTime();
    state->deltaTime = now - timeElapsed;
    timeElapsed = now;

    glfwPollEvents();

    sceneManager.update();
    state->renderer->update();

    state->fps = 1000 / (t.elapsedNs() / 1000000.0);
}

void Client::cleanup() {
    sceneManager.cleanupScene();
    delete state;
    delete renderer;
}

Client::~Client() {
    cleanup();
}