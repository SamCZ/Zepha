//
// Created by aurailus on 17/12/18.
//

#ifndef SRC_GAMEWORLD_H
#define SRC_GAMEWORLD_H


#include "world/World.h"
#include "../engine/Camera.h"
#include "../engine/Window.h"
#include "../engine/PerlinNoise.h"
#include "../engine/graphics/Renderer.h"
#include "../engine/helpers/ArrayTrans3D.h"
#include "../lua_api/LuaParser.h"
#include "../engine/graphics/HudText.h"
#include "gui/DebugGui.h"
#include "world/Player.h"
#include "../engine/Ray.h"
#include "../client/Scene.h"

class GameScene : public Scene {
public:
    GameScene(ClientState* state);

    void update() override;

    void draw() override;
public:
    Player* player;

    //The World object represents the physical game region that is played in. It handles chunk updates and entities.
    World* world;

    //The texture atlas makes a single patched texture from an asset folder.
    TextureAtlas* textureAtlas;

    //The block atlas holds block definitions and models.
    BlockAtlas* blockAtlas;

    //Entities
    std::vector<Entity*> entities;

    //GUI Related things
    std::vector<Entity*> guiEntities;
    DebugGui gui;
};


#endif //SRC_GAMEWORLD_H
