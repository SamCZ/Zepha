//
// Created by aurailus on 18/04/19.
//

#ifndef ZEUS_GAMEDEFS_H
#define ZEUS_GAMEDEFS_H

#include "texture/TextureAtlas.h"
#include "block/BlockAtlas.h"
#include "../api/LuaApi.h"

class GameDefs {
public:
    GameDefs() = default;
    explicit GameDefs(std::string tex_path);

    BlockAtlas& blocks();
    TextureAtlas& textures();

    ~GameDefs() = default;
private:
    TextureAtlas textureAtlas;
    BlockAtlas blockAtlas;
    LuaApi luaApi;
};


#endif //ZEUS_GAMEDEFS_H
