//
// Created by aurailus on 06/01/19.
//

#include "Client.h"

Client::Client(char* path, int width, int height) :
    renderer(width, height),
    state {renderer, LocalDefs("../res/tex"), 0, 0} {

//    Start Local Server
//    if (path != nullptr) {
//        int pid = fork();
//        if (pid == 0) {
//            char *arr[] =
//                {(char*)"", (char*)"-iconic", (char*)"-e", path, (char*)"--mode=server", (char*)nullptr};
//
//            execvp("xterm", arr);
//        } else {
//            serverPID = pid;
//        }
//    }

    std::unique_ptr<Scene> scene = std::make_unique<MenuScene>(state);
    sceneManager.setScene(std::move(scene));

    while (!renderer.getWindow().shouldClose()) loop();
}

void Client::loop() {
    Timer t("Client Loop");

    if (!startedGame && timeElapsed > 1) {
//        std::unique_ptr<Scene> scene = std::make_unique<ConnectScene>(state);
        std::unique_ptr<Scene> scene = std::make_unique<GameScene>(state);
        sceneManager.setScene(std::move(scene));
        startedGame = true;
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
    if (serverPID != 0) kill(serverPID, SIGKILL);
    sceneManager.cleanupScene();
}