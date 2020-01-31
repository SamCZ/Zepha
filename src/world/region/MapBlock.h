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
    MapBlock(glm::ivec3 pos);

    std::shared_ptr<BlockChunk> operator[](int index);
    void set(int index, std::shared_ptr<BlockChunk> chunk);

    bool generated = false;
private:
    glm::ivec3 pos {};
    std::array<std::shared_ptr<BlockChunk>, 64> blockChunks;
};
