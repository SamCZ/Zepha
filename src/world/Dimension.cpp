//
// Created by aurailus on 2019-11-20.
//

#include "Dimension.h"

std::shared_ptr<BlockChunk> Dimension::getChunk(glm::vec3 pos) {
    if (blockChunks.count(pos)) return blockChunks.at(pos);
    return nullptr;
}

void Dimension::setChunk(std::shared_ptr<BlockChunk> chunk) {
    blockChunks.insert({chunk->pos, chunk});
}

unsigned int Dimension::getBlock(glm::vec3 pos) {
    auto chunkPos = TransPos::chunkFromVec(TransPos::roundPos(pos));
    auto local = TransPos::chunkLocalFromVec(TransPos::roundPos(pos));

    auto chunk = getChunk(chunkPos);
    if (chunk != nullptr) return chunk->getBlock(local);
    return 0;
}

bool Dimension::setBlock(glm::vec3 pos, uint block) {
    auto chunkPos = TransPos::chunkFromVec(TransPos::roundPos(pos));
    auto local = TransPos::chunkLocalFromVec(TransPos::roundPos(pos));

    auto chunk = getChunk(chunkPos);
    if (chunk == nullptr) return false;

    chunk->setBlock(local, block);
    return true;
}