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
    Region(glm::vec3 pos);

    std::shared_ptr<MapBlock> operator[](int index);
    void set(int index, std::shared_ptr<MapBlock> block);
private:
    glm::vec3 pos {};
    std::array<std::shared_ptr<MapBlock>, 64> mapBlocks {};
};
