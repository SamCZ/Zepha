//
// Created by aurailus on 2019-11-18.
//

#pragma once

#include <string>
#include "BiomeDef.h"
#include "BiomeAtlas.h"
#include "../../util/Log.h"

class LocalBiomeAtlas : public BiomeAtlas {
public:
    LocalBiomeAtlas();
    void registerBiome(BiomeDef* def) override;

    void setIdentifiers(std::vector<std::string>& identifiers);
};
