//
// Created by aurailus on 01/10/19.
//

#include "ServerDimension.h"

bool ServerDimension::setBlock(glm::vec3 pos, unsigned int block) {
    bool manip = Dimension::setBlock(pos, block);
    if (!manip) return false;
    glm::vec3 mb = Space::MapBlock::world::fromBlock(pos);
    mapBlockIntegrity[mb] = mapBlockIntegrity[mb] + 1;
    return true;
}

void ServerDimension::setChunk(std::shared_ptr<BlockChunk> chunk) {
    Dimension::setChunk(chunk);
    glm::vec3 mb = Space::MapBlock::world::fromChunk(chunk->pos);
    mapBlockIntegrity[mb] = mapBlockIntegrity[mb] + 1;
}

unsigned long long ServerDimension::getMapBlockIntegrity(glm::vec3 mapBlock) {
    if (mapBlockIntegrity.count(mapBlock)) return mapBlockIntegrity[mapBlock];
    return 0;
}
