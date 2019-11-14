//
// Created by aurailus on 2019-11-13.
//

#include "BiomeDef.h"

BiomeDef::BiomeDef(const std::string& identifier, float temperature, float humidity, float roughness, unsigned int topBlock, unsigned int soilBlock, unsigned int rockBlock) :
        identifier(identifier),

        temperature(temperature),
        humidity(humidity),
        roughness(roughness),

        topBlock(topBlock),
        soilBlock(soilBlock),
        rockBlock(rockBlock) {}
