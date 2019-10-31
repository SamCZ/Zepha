//
// Created by aurailus on 06/01/19.
//

#pragma once

#include "ClientState.h"
#include "graph/Renderer.h"
#include "graph/scene/SceneManager.h"
#include "../util/Timer.h"
#include "scene/GameScene.h"
#include "scene/MenuScene.h"
#include "scene/ConnectScene.h"
#include "../server/LocalServerInstance.h"

class Client {
public:
    Client(const Address& addr, glm::vec2 dimensions);
    Client(uptr<LocalServerInstance> localServer, glm::vec2 dimensions);
    ~Client();

private:
    void init();
    void loop();

    uptr<LocalServerInstance> localServer = nullptr;
    Address addr {};

    Renderer renderer;
    ClientState state;

    SceneManager sceneManager;

    double timeElapsed = 0.0f;
    bool startedGame = false;
};

