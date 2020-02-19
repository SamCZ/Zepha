//
// Created by aurailus on 18/04/19.
//

#pragma once

#include "LocalDefinitionAtlas.h"
#include "gen/LocalBiomeAtlas.h"
#include "model/ModelStore.h"
#include "texture/TextureAtlas.h"
#include "../lua/parser/LocalLuaParser.h"

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

