//
// Created by aurailus on 02/12/18.
//

#include "BlockAtlas.h"

BlockAtlas::BlockAtlas(BlockModel *model) {
    blockModel = model;
}

BlockModel* BlockAtlas::getBlockModel() {
    return blockModel;
}
