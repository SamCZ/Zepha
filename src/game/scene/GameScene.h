//
// Created by aurailus on 17/12/18.
//

#pragma once

#include "net/ClientNetworkInterpreter.h"
#include "world/LocalWorld.h"
#include "world/Player.h"
#include "../graph/scene/Scene.h"
#include "../graph/Renderer.h"
#include "../hud/DebugGui.h"
#include "../hud/GameGui.h"
#include "../entity/world/PlayerEntity.h"
#include "../entity/world/WireframeEntity.h"
#include "../entity/world/BlockCrackEntity.h"
#include "../entity/Model.h"
#include "../../def/LocalDefs.h"

class GameScene : public Scene {
public:
    explicit GameScene(ClientState& state);

    void update() override;
    void draw() override;

    void cleanup() override;
public:
    LocalDefs& defs;

    glm::vec3 playerPos;
    Player player;

    ClientNetworkInterpreter server;
    LocalWorld world;

    std::vector<Drawable*> entities;

    DebugGui debugGui;

    int drawCalls = 0;

    bool debugVisible = true;
    bool hudVisible = true;
};

