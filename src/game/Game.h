//
// Created by aurailus on 06/01/19.
//

#ifndef ZEUS_CLIENT_H
#define ZEUS_CLIENT_H

#include "ClientState.h"
#include "graph/Renderer.h"
#include "graph/scene/SceneManager.h"

#include "scene/GameScene.h"
#include "scene/MenuScene.h"

#include "../util/Timer.h"

class Game {
public:
    Game();
    Game(int width, int height, char* path);
    ~Game();

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
