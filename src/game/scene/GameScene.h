//
// Created by aurailus on 17/12/18.
//

#ifndef SRC_GAMEWORLD_H
#define SRC_GAMEWORLD_H

#include "../graph/scene/Scene.h"
#include "../graph/Renderer.h"
#include "../entity/hud/DebugGui.h"
#include "../entity/hud/GameGui.h"
#include "world/LocalWorld.h"
#include "world/Player.h"
#include "net/ServerConnection.h"
#include "../entity/world/PlayerEntity.h"
#include "../entity/world/WireframeEntity.h"
#include "../entity/world/BlockCrackEntity.h"
#include "../../def/LocalDefs.h"

class GameScene : public Scene {
public:
    explicit GameScene(ClientState* state);

    void update() override;
    void draw() override;

    void cleanup() override;
public:
    LocalDefs defs;

    glm::vec3 playerPos;

    ServerConnection server;
    Player player;
    LocalWorld world;

    std::vector<Drawable*> entities;
    std::vector<Drawable*> gui;

    DebugGui debugGui;
    GameGui gameGui;

    int drawCalls;

    bool debugVisible = true;
    bool hudVisible = true;
};


#endif //SRC_GAMEWORLD_H
