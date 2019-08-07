//
// Created by aurailus on 06/01/19.
//

#ifndef ZEUS_CLIENT_H
#define ZEUS_CLIENT_H

#include <zconf.h>
#include <signal.h>
#include "ClientState.h"
#include "graph/Renderer.h"
#include "graph/scene/SceneManager.h"
#include "../util/Timer.h"
#include "scene/GameScene.h"
#include "scene/MenuScene.h"
#include "scene/ConnectScene.h"

class Client {
public:
    Client(char* path, const Address& addr, int width, int height);
    ~Client();

private:
    void loop();

    Address addr;

    Renderer renderer;
    ClientState state;

    SceneManager sceneManager;

    double timeElapsed = 0.0f;
    bool startedGame = false;

    int serverPID = 0;
};


#endif //ZEUS_CLIENT_H
