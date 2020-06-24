//
// Created by aurailus on 22/04/19.
//

#include "MapBlock.h"

#include "Chunk.h"

MapBlock::MapBlock(glm::ivec3 pos) :
    pos(pos) {
    for(unsigned short i = 0; i < 64; i++) blockChunks[i] = nullptr;
}

std::shared_ptr<Chunk> MapBlock::operator[](unsigned short index) {
    return blockChunks[index];
}

void MapBlock::set(unsigned short index, std::shared_ptr<Chunk> chunk) {
    if (blockChunks[index] == nullptr) count++;
    blockChunks[index] = chunk;
}

void MapBlock::remove(unsigned short index) {
    if (blockChunks[index] != nullptr) count--;
    blockChunks[index] = nullptr;
}
