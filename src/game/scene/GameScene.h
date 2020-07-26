//
// Created by aurailus on 17/12/18.
//

#pragma once

#include "../graph/scene/Scene.h"

#include "world/Player.h"
#include "world/LocalWorld.h"
#include "../../net/client/ClientNetworkInterpreter.h"
#include "../inventory/LocalInventoryRefs.h"
#include "../hud/DebugGui.h"

class LocalSubgame;
class Drawable;

class GameScene : public Scene {
public:
    explicit GameScene(ClientState& state);

    void update() override;
    void draw() override;

    void cleanup() override;
public:
    LocalSubgame& game;

    ClientNetworkInterpreter net;
    LocalInventoryRefs refs;
    LocalWorld world;
    Player player;

    DebugGui debugGui;
    std::vector<Drawable*> entities;
    int drawCalls = 0;

    bool debugVisible = true;
    bool hudVisible = true;
};

