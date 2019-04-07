//
// Created by aurailus on 04/04/19.
//

#ifndef ZEUS_MAPBLOCK_H
#define ZEUS_MAPBLOCK_H


#include <vec3.hpp>
#include <array>
#include "../helpers/TransPos.h"
#include "../blocks/BlockChunk.h"

class MapBlock {
public:
    explicit MapBlock(glm::vec3 pos);

    BlockChunk* operator[](int index);
    void set(int index, BlockChunk* block);

    ~MapBlock();
private:
    glm::vec3 pos;

    const static int arrayLength = TransPos::MAPBLOCK_SIZE * TransPos::MAPBLOCK_SIZE * TransPos::MAPBLOCK_SIZE;
    std::array<BlockChunk*, arrayLength> blockChunks;
};


#endif //ZEUS_MAPBLOCK_H
