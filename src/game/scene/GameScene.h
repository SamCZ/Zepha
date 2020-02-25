//
// Created by aurailus on 17/12/18.
//

#pragma once

#include "world/Player.h"
#include "../ClientState.h"
#include "../hud/DebugGui.h"
#include "../graph/scene/Scene.h"

class GameScene : public Scene {
public:
    explicit GameScene(ClientState& state);

    void update() override;
    void draw() override;

    void cleanup() override;
public:
    ClientGame& game;

    ClientNetworkInterpreter net;
    LocalInventoryRefs refs;
    LocalWorld world;

    glm::vec3 playerPos;
    Player player;

    std::vector<Drawable*> entities;
    DebugGui debugGui;

    int drawCalls = 0;

    bool debugVisible = true;
    bool hudVisible = true;
};

