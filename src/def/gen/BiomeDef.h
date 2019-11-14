//
// Created by aurailus on 2019-11-13.
//

#pragma once

#include <string>

struct BiomeDef {
    BiomeDef() = default;
    BiomeDef(const std::string& identifier, unsigned int index, float temperature, float humidity, float roughness,
            unsigned int topBlock, unsigned int soilBlock, unsigned int rockBlock);

    std::string identifier;
    unsigned int index;

    float temperature = 0;
    float humidity = 0;
    float roughness = 0;

    unsigned int topBlock;
    unsigned int soilBlock;
    unsigned int rockBlock;
};
