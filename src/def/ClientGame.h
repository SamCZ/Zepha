//
// Created by aurailus on 18/04/19.
//

#pragma once

#include "gen/MapGenProps.h"
#include "model/ModelStore.h"
#include "gen/LocalBiomeAtlas.h"
#include "texture/TextureAtlas.h"
#include "LocalDefinitionAtlas.h"
#include "../lua/parser/LocalLuaParser.h"

class ClientGame {
public:
    explicit ClientGame(const std::string& tex_path);
    ClientGame(const ClientGame& copy);

    void init(LocalWorld &world, Player& player);
    void update(double delta, bool* keys);

    ~ClientGame() = default;

    std::string tex_path;

    TextureAtlas         textures;
    LocalLuaParser       parser;
    LocalDefinitionAtlas defs;
    LocalBiomeAtlas      biomes;
    ModelStore           models;
};

