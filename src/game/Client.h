//
// Created by aurailus on 06/01/19.
//

#pragma once

#include "ClientState.h"
#include "graph/Renderer.h"
#include "graph/scene/SceneManager.h"
#include "../util/Timer.h"
#include "scene/GameScene.h"
#include "scene/MainMenuScene.h"
#include "scene/ConnectScene.h"
#include "../server/LocalServerInstance.h"

class Client {
public:
    Client(const std::string& path, const Address& addr, glm::ivec2 dims);
    ~Client();

private:
    void loop();

    Address addr {};
    Renderer renderer;
    ClientState state;
    SceneManager sceneManager;

    double timeElapsed = 0.0f;
};

