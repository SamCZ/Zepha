//
// Created by aurailus on 2019-11-18.
//

#pragma once

#include "BiomeAtlas.h"

class LocalBiomeAtlas : public BiomeAtlas {
public:
    LocalBiomeAtlas();
    void registerBiome(BiomeDef* def) override;
    void setIdentifiers(const std::vector<std::string>& identifiers);
};
