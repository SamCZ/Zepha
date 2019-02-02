//
// Created by aurailus on 06/01/19.
//

#ifndef ZEUS_CLIENT_H
#define ZEUS_CLIENT_H

#include "engine/graphics/Renderer.h"
#include "ClientState.h"
#include "engine/scene/SceneManager.h"

#include "scene/GameScene.h"
#include "scene/MenuScene.h"

#include "engine/Timer.h"

class Client {
public:
    Client();
    Client(int width, int height);

    void start(char* path);

    ~Client();

private:
    void loop();

    void cleanup();

    Renderer* renderer;

    ClientState* state;
    SceneManager sceneManager;

    int local_server_pid = 0;
    int count = 0;

    double timeElapsed = 0.0f;
};


#endif //ZEUS_CLIENT_H
