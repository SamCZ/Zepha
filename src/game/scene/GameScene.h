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

    LocalInventoryRefs refs;
    ClientNetworkInterpreter net;
    LocalWorld world;
    Player player;

    DebugGui debugGui;
    std::vector<Drawable*> entities;
    int drawCalls = 0;

    bool debugVisible = true;
    bool hudVisible = true;
};

