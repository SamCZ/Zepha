//
// The ClientGame class stores all of the subgame data for the client.
// This data is used when in the GameScene. It is initialized when the client joins a game and cleared when they exit it.
// The data within is in an undefined state until the init method is called.
// Created by aurailus on 18/04/19.
//

#pragma once

#include "gen/MapGenProps.h"
#include "model/ModelStore.h"
#include "gen/LocalBiomeAtlas.h"
#include "texture/TextureAtlas.h"
#include "LocalDefinitionAtlas.h"
#include "../lua/parser/LocalLuaParser.h"

class ClientState;

class ClientGame {
public:
    explicit ClientGame(const std::string& tex_path);
    // This constructor is only valid for ClientGame objects!
    ClientGame(const ClientGame& copy);

    void init(LocalWorld &world, Player& player, ClientState& state);
    void update(double delta);

    ~ClientGame() = default;

    std::string tex_path;

    TextureAtlas         textures;
    LocalLuaParser       parser;
    LocalDefinitionAtlas defs;
    LocalBiomeAtlas      biomes;
    ModelStore           models;
};

