//
// Created by aurailus on 04/04/19.
//

#include "Dimension.h"

void Dimension::addChunk(glm::vec3 pos, std::shared_ptr<BlockChunk> chunk) {

    glm::vec3 regionPos = TransPos::Dimension::regionFromVec(pos);
    glm::vec3 regionRawPos = TransPos::Dimension::regionRawFromRegionVec(pos);

    if (!regions.count(regionPos)) regions.insert({regionPos, new Region(regionPos, regionRawPos)});
    Region* region = regions[regionPos];

    glm::vec3 mapBlockPos = TransPos::Dimension::mapBlockOffsetFromRegion(pos);
    unsigned int mapBlockInd = TransPos::mapBlockIndFromVec(mapBlockPos);

    //TODO: Don't use 0, 0, 0
    if ((*region)[mapBlockInd] == nullptr) region->set(mapBlockInd, new MapBlock(mapBlockPos, {0, 0, 0}));
    MapBlock* mapBlock = (*region)[mapBlockInd];

    glm::vec3 chunkPos = TransPos::Dimension::chunkOffsetFromMapBlock(pos);
    unsigned int chunkInd = TransPos::chunkIndFromVec(chunkPos);
    mapBlock->set(chunkInd, std::move(chunk));
}

std::shared_ptr<BlockChunk> Dimension::getChunk(glm::vec3 pos) {

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

Region* Dimension::getRegion(glm::vec3 pos) {
    return regions[pos];
}

Dimension::region_map &Dimension::getRegions() {
    return regions;
}

Dimension::~Dimension() {
    for (const auto &region : regions) {
        delete region.second;
    }
}