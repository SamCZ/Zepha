//
// Created by aurailus on 18/04/19.
//

#ifndef ZEUS_GAMEDEFS_H
#define ZEUS_GAMEDEFS_H

#include "texture/TextureAtlas.h"
#include "block/client/LocalBlockAtlas.h"
#include "../api/client/LocalLuaParser.h"

class LocalDefs {
public:
    explicit LocalDefs(std::string tex_path);
    LocalDefs(const LocalDefs& copy);

    LocalBlockAtlas& blocks();
    TextureAtlas& textures();
    LocalLuaParser& lua();

    void init(LocalWorld& world);
    void update(float delta);

    ~LocalDefs() = default;
private:
    float delta = 0;

    std::string tex_path;

    TextureAtlas textureAtlas;
    LocalBlockAtlas blockAtlas;
    LocalLuaParser luaApi;
};


#endif //ZEUS_GAMEDEFS_H
