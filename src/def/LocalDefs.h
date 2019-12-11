//
// Created by aurailus on 18/04/19.
//

#pragma once

#include "texture/TextureAtlas.h"
#include "LocalDefinitionAtlas.h"
#include "../lua/client/LocalLuaParser.h"
#include "model/ModelStore.h"
#include "gen/LocalBiomeAtlas.h"

class LocalDefs {
public:
    explicit LocalDefs(const std::string& tex_path);
    LocalDefs(const LocalDefs& copy);

    void init(LocalWorld &world, Player& player);
    void update(double delta, bool* keys);

    ~LocalDefs() = default;

    std::string tex_path;

    TextureAtlas         textures;
    LocalLuaParser       luaApi;
    LocalDefinitionAtlas defs;
    LocalBiomeAtlas      biomes;
    ModelStore           models;
};

