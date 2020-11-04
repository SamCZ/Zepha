//
// Created by aurailus on 17/12/18.
//

#pragma once

#include "Scene.h"

#include "world/LocalWorld.h"
#include "client/gui/DebugGui.h"
#include "world/player/LocalPlayer.h"
#include "world/inv/LocalInventoryRefs.h"
#include "client/conn/ClientNetworkInterpreter.h"

class LocalSubgame;
class Drawable;

class GameScene : public Scene {
public:
    GameScene(Client& client);

    void update() override;
    void draw() override;

    void cleanup() override;

public:
    WorldPtr world;

    DebugGui debugGui;
    std::vector<Drawable*> entities;
    int drawCalls = 0;

    bool debugVisible = true;
    bool hudVisible = true;
};

