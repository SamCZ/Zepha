//
// Created by aurailus on 2020-02-01.
//

#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>

class DefinitionAtlas;

struct Schematic {
    std::vector<std::string> stringData {};
    bool processed = false;

    std::vector<unsigned int> blocks {};
    glm::ivec3 dimensions {};
    glm::ivec3 origin {};

    void process(DefinitionAtlas& atlas);

    inline unsigned int length() { return blocks.size(); }

    glm::ivec3 getOffset(unsigned int ind);
    unsigned int index(const glm::ivec3& vec);
};
