//
// Created by aurailus on 18/04/19.
//

#pragma once

#include "texture/TextureAtlas.h"
#include "LocalDefinitionAtlas.h"
#include "../lua/client/LocalLuaParser.h"

class LocalDefs {
public:
    explicit LocalDefs(const std::string& tex_path);
    LocalDefs(const LocalDefs& copy);

    LocalDefinitionAtlas& defs();
    TextureAtlas& textures();
    LocalLuaParser& lua();

    void initLuaApi(LocalWorld &world, GameGui& gui);
    void update(float delta);

    ~LocalDefs() = default;
private:
    float delta = 0;

    std::string tex_path;

    TextureAtlas textureAtlas;
    LocalDefinitionAtlas blockAtlas;
    LocalLuaParser luaApi;
};

