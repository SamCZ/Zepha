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

    void init(LocalWorld& world);

    LocalBlockAtlas& blocks();
    TextureAtlas& textures();
    LocalLuaParser& lua();

    ~LocalDefs() = default;
private:
    TextureAtlas textureAtlas;
    LocalBlockAtlas blockAtlas;
    LocalLuaParser luaApi;
};


#endif //ZEUS_GAMEDEFS_H
