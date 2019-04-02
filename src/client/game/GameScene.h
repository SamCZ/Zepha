//
// Created by aurailus on 17/12/18.
//

#ifndef SRC_GAMEWORLD_H
#define SRC_GAMEWORLD_H


#include "../engine/scene/Scene.h"
#include "../engine/graphics/Renderer.h"
#include "../graphics/gui/DebugGui.h"
#include "../graphics/gui/GameGui.h"

#include "../lua/LuaParser.h"

#include "localworld/LocalWorld.h"
#include "Player.h"

#include "../network/ServerConnection.h"

#include "../../generic/blocks/TextureAtlas.h"
#include "../../generic/blocks/BlockAtlas.h"
#include "entity/PlayerEntity.h"

class GameScene : public Scene {
public:
    explicit GameScene(ClientState* state);

    void update() override;

    void draw() override;

    void cleanup() override;
public:
    ServerConnection* server;
    Player* player;
    LocalWorld* world;
    TextureAtlas* textureAtlas;
    BlockAtlas* blockAtlas;

    //Entities to be drawn with entity shaders
    std::vector<Entity*> entities;

    std::vector<PlayerEntity*> playerEntities;

    //GUI Related things
    std::vector<Entity*> guiEntities;
    DebugGui debugGui;
    GameGui gui;

    int drawCalls;

    bool F1Down = false;
    bool F3Down = false;

    bool debugVisible = true;
    bool hudVisible = true;
};


#endif //SRC_GAMEWORLD_H
