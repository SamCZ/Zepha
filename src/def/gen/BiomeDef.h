//
// Created by aurailus on 2019-11-13.
//

#pragma once

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <noise/module/modulebase.h>

struct BiomeDef {
    BiomeDef() = default;
    BiomeDef(const std::string& identifier, unsigned int index, float temperature, float humidity, float roughness,
            unsigned int topBlock, unsigned int soilBlock, unsigned int rockBlock, const std::vector<noise::module::Module*>& modules, glm::vec3 biomeTint);

    std::string identifier = "";
    unsigned int index = 0;

    float temperature = 0;
    float humidity = 0;
    float roughness = 0;

    unsigned int topBlock = 0;
    unsigned int soilBlock = 0;
    unsigned int rockBlock = 0;

    std::vector<noise::module::Module*> modules;

    glm::vec3 biomeTint {};
};
