//
// Created by aurailus on 17/12/18.
//

#pragma once

#include "net/ClientNetworkInterpreter.h"
#include "world/LocalWorld.h"
#include "world/Player.h"
#include "../graph/scene/Scene.h"
#include "../graph/Renderer.h"
#include "../entity/hud/DebugGui.h"
#include "../entity/hud/GameGui.h"
#include "../entity/world/PlayerEntity.h"
#include "../entity/world/WireframeEntity.h"
#include "../entity/world/BlockCrackEntity.h"
#include "../../def/LocalDefs.h"
#include "../../def/entity/Model.h"

class GameScene : public Scene {
public:
    explicit GameScene(ClientState& state);

    void update() override;
    void draw() override;

    void cleanup() override;
public:
    LocalDefs& defs;

    glm::vec3 playerPos;

    ClientNetworkInterpreter server;
    Player player;
    LocalWorld world;

    std::vector<Drawable*> entities;
    std::vector<Drawable*> gui;

    DebugGui debugGui;
    GameGui gameGui;

    Entity* rabbit;
    double time = 1.f/60.f;

    int drawCalls = 0;

    bool debugVisible = true;
    bool hudVisible = true;
};

