//
// Created by aurailus on 18/04/19.
//

#pragma once

#include <glm/vec3.hpp>
#include <array>
#include <memory>
#include "../../util/TransPos.h"
#include "../chunk/BlockChunk.h"

class MapBlock {
public:
    MapBlock(glm::vec3 pos, glm::vec3 rawPos);

    std::shared_ptr<BlockChunk> operator[](int index);
    void set(int index, std::shared_ptr<BlockChunk> chunk);

    ~MapBlock() = default;
private:
    glm::vec3 pos {}, rawPos {};

    const static int arrayLength = TransPos::MAPBLOCK_SIZE * TransPos::MAPBLOCK_SIZE * TransPos::MAPBLOCK_SIZE;
    std::array<std::shared_ptr<BlockChunk>, arrayLength> blockChunks;
};
