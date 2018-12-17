//
// Created by aurailus on 02/12/18.
//

#ifndef GLPROJECT_BLOCKATLAS_H
#define GLPROJECT_BLOCKATLAS_H


#include "../mesh/BlockModel.h"
#include "BlockDef.h"

class BlockAtlas {
public:
    BlockAtlas() = default;
    explicit BlockAtlas(TextureAtlas* textureAtlas);

    void registerBlock(BlockDef* def);
    BlockDef* getBlock(int id);

    ~BlockAtlas();
private:
    std::vector<BlockDef*> definitions;
    TextureAtlas* textureAtlas;
};


#endif //GLPROJECT_BLOCKATLAS_H
