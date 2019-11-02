//
// Created by aurailus on 01/10/19.
//

#include "ServerDimension.h"

typedef unsigned int uint;

void ServerDimension::setChunk(sptr<BlockChunk> chunk) {
    blockChunks.insert({chunk->pos, chunk});
}

std::shared_ptr<BlockChunk> ServerDimension::getChunk(glm::vec3 pos) {
    if (blockChunks.count(pos)) return blockChunks.at(pos);
    return nullptr;
}

void ServerDimension::setBlock(glm::vec3 pos, uint block) {
    auto chunkPos = TransPos::chunkFromVec(TransPos::roundPos(pos));
    auto local = TransPos::chunkLocalFromVec(TransPos::roundPos(pos));

    auto chunk = getChunk(chunkPos);
    if (chunk != nullptr) chunk->setBlock(local, block);
}

uint ServerDimension::getBlock(glm::vec3 pos) {
    auto chunkPos = TransPos::chunkFromVec(TransPos::roundPos(pos));
    auto local = TransPos::chunkLocalFromVec(TransPos::roundPos(pos));

    auto chunk = getChunk(chunkPos);
    if (chunk != nullptr) return chunk->getBlock(local);
    return 0;
}