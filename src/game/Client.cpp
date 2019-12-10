//
// Created by aurailus on 06/01/19.
//

#include "Client.h"

Client::Client(const Address &addr, glm::vec2 dimensions) :
    addr(addr),
    renderer(dimensions.x, dimensions.y),
    state {renderer, {}, LocalDefs("./assets/textures"), "this", 0, 0} {
    init();
}

Client::Client(uptr<LocalServerInstance> localServer, glm::vec2 dimensions) :
    localServer(std::move(localServer)),
    addr(Address {"127.0.0.1", this->localServer->port}),
    renderer(dimensions.x, dimensions.y),
    state {renderer, {}, LocalDefs("./assets/textures"), "this", 0, 0} {
    init();
}

void Client::init() {
    std::cout << Log::info << "Starting Zepha Client." << Log::endl;

//    if (localServer != nullptr) localServer->start();

    std::unique_ptr<Scene> scene = std::make_unique<MainMenuScene>(state);
    sceneManager.setScene(std::move(scene));

    while (!renderer.getWindow().shouldClose()) loop();
}

void Client::loop() {
    Timer t("Client Loop");

    if (state.desiredState == "connect") {
        state.desiredState = "this";
        std::unique_ptr<Scene> scene = std::make_unique<ConnectScene>(state, addr);
        sceneManager.setScene(std::move(scene));
    }

    if (state.desiredState == "game") {
        state.desiredState = "this";
        std::unique_ptr<Scene> scene = std::make_unique<GameScene>(state);
        sceneManager.setScene(std::move(scene));
    }

    double now = glfwGetTime();
    state.deltaTime = now - timeElapsed;
    timeElapsed = now;

    glfwPollEvents();

    sceneManager.update();
    renderer.update(state.deltaTime);

    state.fps = 1000.0 / (t.elapsedNs() / 1000000.0);
}

Client::~Client() {
    if (localServer != nullptr) localServer->stop();
    sceneManager.cleanupScene();
}