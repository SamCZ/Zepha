//
// Created by aurailus on 04/04/19.
//

#pragma once

#include <glm/vec3.hpp>
#include <array>
#include "MapBlock.h"
#include "../../util/Space.h"

class Region {
public:
    Region(glm::ivec3 pos);

    std::shared_ptr<MapBlock> operator[](unsigned short index);
    void set(unsigned short index, std::shared_ptr<MapBlock> block);
    void remove(unsigned short index);

    glm::ivec3 pos {};
    unsigned short count = 0;
private:
    std::array<std::shared_ptr<MapBlock>, 64> mapBlocks {};
};
