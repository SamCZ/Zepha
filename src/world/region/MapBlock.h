//
// Created by aurailus on 18/04/19.
//

#pragma once

#include <array>
#include <memory>
#include <glm/vec3.hpp>

#include "../chunk/BlockChunk.h"

class MapBlock {
public:
    MapBlock(glm::ivec3 pos);

    std::shared_ptr<BlockChunk> operator[](unsigned short index);
    void set(unsigned short index, std::shared_ptr<BlockChunk> chunk);
    void remove(unsigned short index);

    glm::ivec3 pos {};
    bool generated = false;
    unsigned short count = 0;
private:
    std::array<std::shared_ptr<BlockChunk>, 64> blockChunks;
};
