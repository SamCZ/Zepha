//
// Created by aurailus on 04/04/19.
//

#include "MapBlock.h"

MapBlock::MapBlock(glm::vec3 pos) {
    for (int i = 0; i < arrayLength; i++) {
        blockChunks[i] = nullptr;
    }

    this->pos = pos;
}

BlockChunk *MapBlock::operator[](int index) {
    return blockChunks[index];
}

void MapBlock::set(int index, BlockChunk* chunk) {
    blockChunks[index] = chunk;
}

MapBlock::~MapBlock() {
    for (int i = 0; i < arrayLength; i++) {
        delete blockChunks[i];
    }
}
