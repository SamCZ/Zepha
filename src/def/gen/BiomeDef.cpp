//
// Created by aurailus on 2019-11-13.
//

#include "BiomeDef.h"

BiomeDef::BiomeDef(
    const std::string& identifier, unsigned int index, float temperature, float humidity, float roughness,
    unsigned int topBlock, unsigned int soilBlock, unsigned int rockBlock,
    const std::vector<noise::module::Module*>& heightmap, const std::vector<noise::module::Module*>& volume,
    const std::vector<std::shared_ptr<Schematic>> schematics,
    glm::vec3 biomeTint) :

    identifier(identifier),
    index(index),

    temperature(temperature),
    humidity(humidity),
    roughness(roughness),

    topBlock(topBlock),
    soilBlock(soilBlock),
    rockBlock(rockBlock),

    heightmap(heightmap),
    volume(volume),

    schematics(schematics),

    biomeTint(biomeTint) {}
