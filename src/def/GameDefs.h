//
// Created by aurailus on 18/04/19.
//

#ifndef ZEUS_GAMEDEFS_H
#define ZEUS_GAMEDEFS_H

#include "texture/TextureAtlas.h"
#include "block/BlockAtlas.h"

class GameDefs {
public:
    GameDefs() = default;
    explicit GameDefs(std::string tex_path);

    BlockAtlas& getBlockAtlas();
    TextureAtlas& getTextureAtlas();

    ~GameDefs() = default;
private:
    TextureAtlas textures;
    BlockAtlas blocks;
};


#endif //ZEUS_GAMEDEFS_H
