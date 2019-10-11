//
// Created by aurailus on 18/04/19.
//

#pragma once

#include "texture/TextureAtlas.h"
#include "LocalDefinitionAtlas.h"
#include "../lua/client/LocalLuaParser.h"
#include "model/ModelStore.h"

class LocalDefs {
public:
    explicit LocalDefs(const std::string& tex_path);
    LocalDefs(const LocalDefs& copy);

    LocalDefinitionAtlas& defs();
    ModelStore& models();
    TextureAtlas& textures();
    LocalLuaParser& lua();

    void initLuaApi(LocalWorld &world, GameGui& gui);
    void update(double delta);

    ~LocalDefs() = default;
private:
    double delta = 0;

    std::string tex_path;

    TextureAtlas textureAtlas;
    ModelStore modelStore;
    LocalDefinitionAtlas blockAtlas;
    LocalLuaParser luaApi;
};

