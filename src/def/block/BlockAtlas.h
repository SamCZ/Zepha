//
// Created by aurailus on 10/06/19.
//

#ifndef ZEUS_BLOCKATLAS_H
#define ZEUS_BLOCKATLAS_H

#include <string>
#include "BlockDef.h"

class BlockAtlas {
public:
    virtual int definitionsSize() = 0;

    virtual BlockDef& fromIndex(int index) = 0;
    virtual BlockDef& fromIdentifier(std::string identifier) = 0;
};

#endif //ZEUS_BLOCKATLAS_H
