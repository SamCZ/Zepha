//
// Created by aurailus on 18/04/19.
//

#ifndef ZEUS_MAPBLOCK_H
#define ZEUS_MAPBLOCK_H

#include <vec3.hpp>
#include <array>
#include <memory>
#include "../../util/TransPos.h"
#include "../chunk/BlockChunk.h"

class MapBlock {
public:
    explicit MapBlock(glm::vec3 pos);

    std::shared_ptr<BlockChunk> operator[](int index);
    void set(int index, std::shared_ptr<BlockChunk> chunk);

    ~MapBlock() = default;
private:
    glm::vec3 pos {};

    const static int arrayLength = TransPos::MAPBLOCK_SIZE * TransPos::MAPBLOCK_SIZE * TransPos::MAPBLOCK_SIZE;
    std::array<std::shared_ptr<BlockChunk>, arrayLength> blockChunks;
};

#endif //ZEUS_MAPBLOCK_H
