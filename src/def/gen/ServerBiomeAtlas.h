//
// Created by aurailus on 2019-11-18.
//

#pragma once

#include "BiomeAtlas.h"
#include "BiomeDef.h"

class ServerBiomeAtlas : public BiomeAtlas {
public:
    ServerBiomeAtlas();
    void registerBiome(BiomeDef* def) override;

};
