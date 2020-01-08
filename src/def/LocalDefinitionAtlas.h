//
// Created by aurailus on 02/12/18.
//

#pragma once

#include <vector>
#include "item/BlockDef.h"
#include "texture/TextureAtlas.h"
#include "DefinitionAtlas.h"

class LocalDefinitionAtlas : public DefinitionAtlas {
public:
    LocalDefinitionAtlas(TextureAtlas& atlas);
    void registerDef(ItemDef* def) override;

    void setIdentifiers(const std::vector<std::string>& identifiers);
};
