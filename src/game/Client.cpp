//
// Created by aurailus on 06/01/19.
//

#include "Client.h"

#include "../util/Timer.h"
#include "scene/GameScene.h"
#include "scene/ConnectScene.h"
#include "scene/MainMenuScene.h"
#include "scene/LuaErrorScene.h"

Client::Client(const std::string& path, const Address &addr, glm::ivec2 dims) :
    state(path.substr(0, path.find_last_of('/') + 1), renderer),
    renderer(dims),
    addr(addr),
    executablePath(path) {

    std::cout << Log::info << "Starting Zepha Client." << Log::endl;

    std::unique_ptr<Scene> scene = std::make_unique<MainMenuScene>(state);
//    std::unique_ptr<Scene> scene = std::make_unique<LuaErrorScene>(state, "whoopsie poopsie did a fucky wucky");
    sceneManager.setScene(std::move(scene));

    while (!renderer.window.shouldClose()) loop();
}

void Client::loop() {
    Timer t("Client Loop");

    if (state.desiredState == "local") {
        state.desiredState = "connect";
        localServer = std::make_unique<LocalServerInstance>(executablePath, addr.port, state.subgame);
        localServer->start();
    }

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
    state.delta = now - timeElapsed;
    timeElapsed = now;

    glfwPollEvents();

    sceneManager.update();
    renderer.update(state.delta);

    state.fps = 1000.0 / (t.elapsedNs() / 1000000.0);
}

Client::~Client() {
    sceneManager.cleanupScene();
    if (localServer) localServer->stop();
}