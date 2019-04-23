//
// Created by aurailus on 22/04/19.
//

#include "MapBlock.h"

MapBlock::MapBlock(glm::vec3 pos, glm::vec3 rawPos) :
    pos(pos),
    rawPos(rawPos) {

    for (int i = 0; i < arrayLength; i++) {
        blockChunks[i] = nullptr;
    }
}

std::shared_ptr<BlockChunk> MapBlock::operator[](int index) {
    return blockChunks[index];
}

void MapBlock::set(int index, std::shared_ptr<BlockChunk> chunk) {
    blockChunks[index] = std::move(chunk);
}