//
// Created by aurailus on 02/12/18.
//

#ifndef GLPROJECT_BLOCKATLAS_H
#define GLPROJECT_BLOCKATLAS_H


#include <vector>
#include "BlockDef.h"
#include "../texture/TextureAtlas.h"

class BlockAtlas {
public:
    BlockAtlas();
    explicit BlockAtlas(TextureAtlas* textureAtlas);

    void registerBlock(BlockDef def);
    BlockDef& getBlock(int id);
private:
    std::vector<BlockDef> definitions;
    TextureAtlas* textureAtlas;
};


#endif //GLPROJECT_BLOCKATLAS_H