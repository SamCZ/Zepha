//
// Created by aurailus on 10/06/19.
//

#pragma once

#include "DefinitionAtlas.h"

class ServerDefinitionAtlas : public DefinitionAtlas {
public:
    ServerDefinitionAtlas();
    void registerDef(ItemDef* def) override;
};
