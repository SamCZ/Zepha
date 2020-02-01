//
// Created by aurailus on 2019-11-20.
//

#include "Dimension.h"

std::shared_ptr<Region> Dimension::getRegion(glm::ivec3 regionPosition) {
    return regions[regionPosition];
}

std::shared_ptr<MapBlock> Dimension::getMapBlock(glm::ivec3 mapBlockPosition) {
    auto region = getRegion(Space::Region::world::fromMapBlock(mapBlockPosition));
    if (region == nullptr) return nullptr;
    return (*region)[Space::MapBlock::index(mapBlockPosition)];
}

std::shared_ptr<BlockChunk> Dimension::getChunk(glm::ivec3 chunkPosition) {
    auto mapBlock = getMapBlock(Space::MapBlock::world::fromChunk(chunkPosition));
    if (mapBlock == nullptr) return nullptr;
    return (*mapBlock)[Space::Chunk::index(chunkPosition)];
}

void Dimension::setChunk(std::shared_ptr<BlockChunk> chunk) {
    auto mapBlock = getOrCreateMapBlock(Space::MapBlock::world::fromChunk(chunk->pos));
    (*mapBlock).set(Space::Chunk::index(chunk->pos), chunk);
}

unsigned int Dimension::getBlock(glm::ivec3 pos) {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    if (chunk != nullptr) return chunk->getBlock(Space::Block::relative::toChunk(pos));
    return 0;
}

bool Dimension::setBlock(glm::ivec3 pos, unsigned int block) {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    if (chunk == nullptr) return false;

    chunk->setBlock(Space::Block::relative::toChunk(pos), block);
    return true;
}

std::shared_ptr<Region> Dimension::getOrCreateRegion(glm::ivec3 pos) {
    if (regions[pos]) return regions[pos];
    regions[pos] = std::make_shared<Region>(pos);
    return regions[pos];
}

std::shared_ptr<MapBlock> Dimension::getOrCreateMapBlock(glm::ivec3 mapBlockPosition) {
    auto region = getOrCreateRegion(Space::Region::world::fromMapBlock(mapBlockPosition));
    unsigned int index = Space::MapBlock::index(mapBlockPosition);

    auto mapBlock = (*region)[index];
    if (mapBlock) return mapBlock;
    (*region).set(index, std::make_shared<MapBlock>(mapBlockPosition));
    return (*region)[index];
}
