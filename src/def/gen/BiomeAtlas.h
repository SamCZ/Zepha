//
// Manages biome definitions and Voronoi map.
// Created by aurailus on 2019-11-13.
//

#pragma once

#include <unordered_map>

#include "../../util/Voronoi3D.h"

class BiomeDef;

class BiomeAtlas {
public:
    BiomeAtlas() = default;
    virtual void registerBiome(BiomeDef* def) = 0;
    void generateVoronoi();

    BiomeDef& getBiomeAt(float temperature, float humidity, float roughness);

    BiomeDef& biomeFromId(unsigned int index);
    BiomeDef& biomeFromStr(const std::string& identifier);

    unsigned int size();

    const static unsigned int INVALID = 0;
protected:
    std::vector<BiomeDef*> defs;
    std::unordered_map<std::string, unsigned int> defTable;
private:
    constexpr const static unsigned short voronoiSize = 64;
    Voronoi3D voronoi {voronoiSize};
};