//
// Created by aurailus on 04/04/19.
//

#include "Dimension.h"

void Dimension::addChunk(glm::vec3 pos, BlockChunk *chunk) {

    glm::vec3 regionPos = TransPos::Dimension::regionFromVec(pos);
    if (!regions.count(regionPos)) {
        regions.insert({regionPos, new Region(regionPos)});
    }

    Region* region = regions[regionPos];

    glm::vec3 mapBlockPos = TransPos::Dimension::mapBlockOffsetFromRegion(pos);
    unsigned int mapBlockInd = TransPos::mapBlockIndFromVec(mapBlockPos);

    if ((*region)[mapBlockInd] == nullptr) {
        region->set(mapBlockInd, new MapBlock(mapBlockPos));
    }

    MapBlock* mapBlock = (*region)[mapBlockInd];

    glm::vec3 chunkPos = TransPos::Dimension::chunkOffsetFromMapBlock(pos);
    unsigned int chunkInd = TransPos::chunkIndFromVec(chunkPos);

    delete (*mapBlock)[chunkInd];
    mapBlock->set(chunkInd, chunk);
}

BlockChunk *Dimension::getChunk(glm::vec3 pos) {

    glm::vec3 regionPos = TransPos::Dimension::regionFromVec(pos);

    if (!regions.count(regionPos)) return nullptr;

    Region* region = regions[regionPos];

    glm::vec3 mapBlockPos = TransPos::Dimension::mapBlockOffsetFromRegion(pos);
    unsigned int mapBlockInd = TransPos::mapBlockIndFromVec(mapBlockPos);

    if ((*region)[mapBlockInd] == nullptr) return nullptr;

    MapBlock* mapBlock = (*region)[mapBlockInd];

    glm::vec3 chunkPos = TransPos::Dimension::chunkOffsetFromMapBlock(pos);
    unsigned int chunkInd = TransPos::chunkIndFromVec(chunkPos);

    return (*mapBlock)[chunkInd];
}

Region *Dimension::getRegion(glm::vec3 pos) {
    return regions[pos];
}

Dimension::~Dimension() {
    for (std::pair<glm::vec3, Region*> region : regions) {
        delete region.second;
    }
}