//
// Created by aurailus on 10/06/19.
//

#ifndef ZEUS_SERVERBLOCKATLAS_H
#define ZEUS_SERVERBLOCKATLAS_H

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "block/BlockDef.h"
#include "DefinitionAtlas.h"

class ServerDefinitionAtlas : public DefinitionAtlas {
public:
    ServerDefinitionAtlas();
    void registerDef(ItemDef* def) override;
};

#endif //ZEUS_SERVERBLOCKATLAS_H
