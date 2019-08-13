//
// Created by aurailus on 02/12/18.
//

#ifndef GLPROJECT_BLOCKATLAS_H
#define GLPROJECT_BLOCKATLAS_H

#include <vector>
#include "block/BlockDef.h"
#include "texture/TextureAtlas.h"
#include "DefinitionAtlas.h"

class LocalDefinitionAtlas : public DefinitionAtlas {
public:
    LocalDefinitionAtlas();
    void registerDef(ItemDef* def) override;

    void setIdentifiers(std::vector<std::string>& identifiers);
};

#endif //GLPROJECT_BLOCKATLAS_H
