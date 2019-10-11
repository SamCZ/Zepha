//
// Created by aurailus on 10/06/19.
//

#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include "item/BlockDef.h"
#include "DefinitionAtlas.h"

class ServerDefinitionAtlas : public DefinitionAtlas {
public:
    ServerDefinitionAtlas();
    void registerDef(ItemDef* def) override;
};
