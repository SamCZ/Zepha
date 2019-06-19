//
// Created by aurailus on 06/01/19.
//

#include <zconf.h>
#include <signal.h>

#include "Game.h"

Game::Game() = default;

Game::Game(int width, int height, char* path) :
    renderer(new Renderer(width, height)) {

    //Start Local Server
    if (path != nullptr) {
        int pid = fork();
        if (pid == 0) {
            char *arr[] = {
                    (char*) "xterm",
                    (char*) "-iconic",
                    (char*) "-e",
                    path,
                    (char*) "server",
                    (char*) nullptr};

            execvp("xterm", arr);
        } else {
            local_server_pid = pid;
        }
    }

    //Create ClientState struct
    state = new ClientState {
            .renderer = renderer,
            .fps = 0,
            .deltaTime = 0
    };

    //Start the Main Menu
    Scene* m = new MenuScene(state);
    sceneManager.setScene(m);

    while (!renderer->getWindow()->getShouldClose()) loop();
}

void Game::loop() {
    //VSync 1 = On, 0 = Off
    glfwSwapInterval(1);

    Timer t("Game Loop");

    double now = glfwGetTime();
    state->deltaTime = now - timeElapsed;
    timeElapsed = now;

    if (count == 5) {
        Scene* g = new GameScene(state);
        sceneManager.setScene(g); //Main Menu Scene here eventually
        count = 8;
    }
    else {
        count++;
    }

    glfwPollEvents();

    sceneManager.update();
    state->renderer->update(state->deltaTime);

    state->fps = 1000 / (t.elapsedNs() / 1000000.0);
}

void Game::cleanup() {
    if (local_server_pid != 0) {
        kill(local_server_pid, SIGKILL);
    }
    sceneManager.cleanupScene();
    delete state;
    delete renderer;
}

Game::~Game() {
    cleanup();
}