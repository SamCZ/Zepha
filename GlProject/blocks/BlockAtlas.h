//
// Created by aurailus on 02/12/18.
//

#ifndef GLPROJECT_BLOCKATLAS_H
#define GLPROJECT_BLOCKATLAS_H


#include "../mesh/BlockModel.h"

class BlockAtlas {
public:
    BlockAtlas() = default;
    explicit BlockAtlas(BlockModel* model);

    BlockModel* getBlockModel();
private:
    BlockModel* blockModel;
};


#endif //GLPROJECT_BLOCKATLAS_H
