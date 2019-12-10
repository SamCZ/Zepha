//
// Created by aurailus on 22/04/19.
//

#include "MapBlock.h"

MapBlock::MapBlock(glm::vec3 pos) :
    pos(pos) {

    for (int i = 0; i < 64; i++) {
        blockChunks[i] = nullptr;
    }
}

std::shared_ptr<BlockChunk> MapBlock::operator[](int index) {
    return blockChunks[index];
}

void MapBlock::set(int index, std::shared_ptr<BlockChunk> chunk) {
    blockChunks[index] = chunk;
}