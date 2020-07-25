//
// The ClientGame class stores all of the subgame data for the client.
// This data is used when in the GameScene. It is initialized when the client joins a game and cleared when they exit it.
// The data within is in an undefined state until the init method is called.
// Created by aurailus on 18/04/19.
//

#pragma once

#include "Game.h"

#include "model/ModelStore.h"
#include "texture/TextureAtlas.h"

class Player;
class LocalWorld;
class ClientState;

class LocalLuaParser;
class LocalBiomeAtlas;
class LocalDefinitionAtlas;

class ClientGame : public Game {
public:
    explicit ClientGame(const std::string& texPath);
    ~ClientGame();

    void init(LocalWorld &world, Player& player, ClientState& state);
    void update(double delta);

    std::string texPath;

    ModelStore models;
    TextureAtlas textures;

    std::unique_ptr<LocalLuaParser> lua;
    std::unique_ptr<LocalBiomeAtlas> biomes;
    std::unique_ptr<LocalDefinitionAtlas> defs;
};

