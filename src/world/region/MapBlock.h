//
// Created by aurailus on 18/04/19.
//

#pragma once

#include <glm/vec3.hpp>
#include <array>
#include <memory>
#include "../../util/Space.h"
#include "../chunk/BlockChunk.h"

class MapBlock {
public:
    MapBlock(glm::vec3 pos);

    std::shared_ptr<BlockChunk> operator[](int index);
    void set(int index, std::shared_ptr<BlockChunk> chunk);
private:
    glm::vec3 pos {};

    const static int arrayLength = std::pow(Space::MAPBLOCK_SIZE, 3);
    std::array<std::shared_ptr<BlockChunk>, arrayLength> blockChunks;
};
