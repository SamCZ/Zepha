//
// Created by aurailus on 2019-11-13.
//

#pragma once

#include <map>
#include <vector>
#include <unordered_map>
#include "BiomeDef.h"
#include "../../util/Voronoi3D.h"

class BiomeStore {
public:
    BiomeStore() = default;
    void registerBiome(BiomeDef* def);
    void generateVoronoi();

    BiomeDef& getBiomeAt(float temperature, float humidity, float roughness);

    BiomeDef& biomeFromId(unsigned int index);
    BiomeDef& biomeFromStr(const std::string& identifier);

    unsigned int size();
private:
    constexpr const static unsigned short voronoiSize = 64;
    Voronoi3D voronoi {voronoiSize};

    std::vector<BiomeDef*> defs;
    std::unordered_map<std::string, unsigned int> defTable;
};