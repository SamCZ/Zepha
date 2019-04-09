//
// Created by aurailus on 06/01/19.
//

//TODO: Move the forking code to main
#include <zconf.h>
#include <signal.h>

#include "Client.h"

Client::Client() = default;

Client::Client(int width, int height) {
    renderer = new Renderer(width, height);
}

void Client::start(char* path) {
    //Start Local Server
    if (path != nullptr) {
        int pid = fork();
        if (pid == 0) {
            char *arr[] = {
                    (char *) "xterm",
                    (char *) "-iconic",
                    (char *) "-e",
                    path,
                    (char *) "server",
                    (char *) nullptr};

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

void Client::loop() {
    //VSync 1 = On, 0 = Off
    glfwSwapInterval(1);

    Timer t("Client Loop");

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
    state->renderer->update();

    state->fps = 1000 / (t.elapsedNs() / 1000000.0);
}

void Client::cleanup() {
    if (local_server_pid != 0) {
        kill(local_server_pid, SIGKILL);
    }
    sceneManager.cleanupScene();
    delete state;
    delete renderer;
}

Client::~Client() {
    cleanup();
}