//
// Created by aurailus on 22/04/19.
//

#include "MapBlock.h"

MapBlock::MapBlock(glm::ivec3 pos) :
    pos(pos) {

    for (unsigned short i = 0; i < 64; i++) {
        blockChunks[i] = nullptr;
    }
}

std::shared_ptr<BlockChunk> MapBlock::operator[](unsigned short index) {
    return blockChunks[index];
}

void MapBlock::set(unsigned short index, std::shared_ptr<BlockChunk> chunk) {
    blockChunks[index] = chunk;
    count++;
}

void MapBlock::remove(unsigned short index) {
    blockChunks[index] = nullptr;
    count--;
}
