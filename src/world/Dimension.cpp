//
// Created by aurailus on 2019-11-20.
//

#include "Dimension.h"

Dimension::Dimension(DefinitionAtlas &defs) : defs(defs) {}

std::shared_ptr<Region> Dimension::getRegion(glm::ivec3 regionPosition) {
    if (!regions.count(regionPosition)) return nullptr;
    return regions[regionPosition];
}

void Dimension::removeRegion(glm::ivec3 pos) {
    regions.erase(pos);
}

std::shared_ptr<MapBlock> Dimension::getMapBlock(glm::ivec3 mapBlockPosition) {
    auto region = getRegion(Space::Region::world::fromMapBlock(mapBlockPosition));
    if (!region) return nullptr;
    return (*region)[Space::MapBlock::index(mapBlockPosition)];
}

void Dimension::removeMapBlock(glm::ivec3 pos) {
    auto region = getRegion(Space::Region::world::fromMapBlock(pos));
    if (!region) return;
    auto ind = Space::MapBlock::index(pos);
    region->remove(ind);
    if (region->count == 0) removeRegion(Space::Region::world::fromMapBlock(pos));
}

std::shared_ptr<BlockChunk> Dimension::getChunk(glm::ivec3 chunkPosition) {
    auto mapBlock = getMapBlock(Space::MapBlock::world::fromChunk(chunkPosition));
    if (!mapBlock) return nullptr;
    return (*mapBlock)[Space::Chunk::index(chunkPosition)];
}

void Dimension::setChunk(std::shared_ptr<BlockChunk> chunk) {
    auto mapBlock = getOrCreateMapBlock(Space::MapBlock::world::fromChunk(chunk->pos));
    (*mapBlock).set(Space::Chunk::index(chunk->pos), chunk);
}

void Dimension::removeChunk(glm::ivec3 pos){
    auto mapBlock = getMapBlock(Space::MapBlock::world::fromChunk(pos));
    if (!mapBlock) return;
    auto ind = Space::Chunk::index(pos);
    mapBlock->remove(ind);
    if (mapBlock->count == 0) removeMapBlock(Space::MapBlock::world::fromChunk(pos));
}

unsigned int Dimension::getBlock(glm::ivec3 pos) {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    if (chunk) return chunk->getBlock(Space::Block::relative::toChunk(pos));
    return 0;
}

bool Dimension::setBlock(glm::ivec3 pos, unsigned int block) {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    if (!chunk) return false;

    chunk->setBlock(Space::Block::relative::toChunk(pos), block);

    glm::ivec3 oldLight = chunk->getBlockLight(Space::Block::index(pos));
    glm::ivec3 newLight = defs.blockFromId(block).lightSource;
    if (oldLight != newLight) {
        if (abs(oldLight.x) + abs(oldLight.y) + abs(oldLight.z) != 0) removeLight(pos);
        if (abs(newLight.x) + abs(newLight.y) + abs(newLight.z) != 0) addLight(pos, newLight);
    }
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

    if ((*region)[index] != nullptr) return (*region)[index];
    (*region).set(index, std::make_shared<MapBlock>(mapBlockPosition));
    return (*region)[index];
}

// Returns true if the provided pos references a block within chunk, otherwise returns false.
bool Dimension::containsWorldPos(BlockChunk *chunk, glm::ivec3 pos) {
    return chunk && Space::Chunk::world::fromBlock(pos) == chunk->pos;
}

// Get the BlockLight of a block. This function can be accelerated
// by providing a chunk that might contain the world position.
glm::ivec3 Dimension::getBlockLight(glm::ivec3 worldPos, BlockChunk *chunk) {
    if (containsWorldPos(chunk, worldPos)) return chunk->getBlockLight(Space::Block::index(worldPos));
    auto oChunk = getChunk(Space::Chunk::world::fromBlock(worldPos)).get();
    return (oChunk ? oChunk->getBlockLight(Space::Block::index(worldPos)) : glm::ivec3 {});
}

void Dimension::addLight(glm::ivec3 pos, glm::ivec3 light) {
    auto startChunk = getChunk(Space::Chunk::world::fromBlock(pos));
    auto index = Space::Block::index(pos);

    startChunk->setBlockLight(index, light);
    lightAddQueue[0].emplace(index, startChunk.get());
    lightAddQueue[1].emplace(index, startChunk.get());
    lightAddQueue[2].emplace(index, startChunk.get());

    propogateAddNodes(0);
    propogateAddNodes(1);
    propogateAddNodes(2);
}

void Dimension::removeLight(glm::ivec3 pos) {
    auto startChunk = getChunk(Space::Chunk::world::fromBlock(pos));
    glm::ivec3 val = startChunk->getBlockLight(Space::Block::index(pos));

    startChunk->setBlockLight(Space::Block::index(pos), {});
    lightRemoveQueue[0].emplace(Space::Block::index(pos), val.r, startChunk.get());
    lightRemoveQueue[1].emplace(Space::Block::index(pos), val.g, startChunk.get());
    lightRemoveQueue[2].emplace(Space::Block::index(pos), val.b, startChunk.get());

    propogateRemoveNodes(0);
    propogateRemoveNodes(1);
    propogateRemoveNodes(2);
}

void Dimension::propogateAddNodes(unsigned char channel) {
    while (!lightAddQueue[channel].empty()) {
        LightAddNode& node = lightAddQueue[channel].front();

        unsigned char lightLevel = node.chunk->getBlockLight(node.index, channel);
        glm::ivec3 worldPos = node.chunk->pos * 16 + Space::Block::fromIndex(node.index);

        const static std::array<glm::ivec3, 6> checks = { glm::ivec3 {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1} };
        for (const auto& i : checks) {
            glm::ivec3 check = worldPos + i;
            if (!defs.blockFromId(getBlock(check)).solid && getBlockLight(check, node.chunk)[channel] + 2 <= lightLevel) {
                auto chunk = containsWorldPos(node.chunk, check) ? node.chunk : getChunk(Space::Chunk::world::fromBlock(check)).get();
                if (!chunk) continue;
                chunk->setBlockLight(Space::Block::index(check), channel, lightLevel - 1);
                lightAddQueue[channel].emplace(Space::Block::index(check), chunk);
            }
        }
        lightAddQueue[channel].pop();
    }
}

void Dimension::propogateRemoveNodes(unsigned char channel) {
    while (!lightRemoveQueue[channel].empty()) {
        LightRemoveNode& node = lightRemoveQueue[channel].front();

        glm::ivec3 worldPos = node.chunk->pos * 16 + Space::Block::fromIndex(node.index);

        const static std::array<glm::ivec3, 6> checks = { glm::ivec3 {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1} };
        for (const auto& i : checks) {
            glm::ivec3 check = worldPos + i;
            unsigned char checkLight = getBlockLight(check, node.chunk)[channel];

            if (checkLight != 0 && checkLight < node.value) {
                auto chunk = containsWorldPos(node.chunk, check) ? node.chunk : getChunk(Space::Chunk::world::fromBlock(check)).get();
                if (!chunk) continue;

                auto blockLight = defs.blockFromId(chunk->getBlock(Space::Block::index(check))).lightSource[channel];
                chunk->setBlockLight(Space::Block::index(check), channel, blockLight);
                if (blockLight) lightAddQueue[channel].emplace(Space::Block::index(check), chunk);
                lightRemoveQueue[channel].emplace(Space::Block::index(check), checkLight, chunk);
            }
            else if (checkLight >= node.value) {
                auto chunk = containsWorldPos(node.chunk, check) ? node.chunk : getChunk(Space::Chunk::world::fromBlock(check)).get();
                if (!chunk) continue;
                lightAddQueue[channel].emplace(Space::Block::index(check), chunk);
            }
        }
        lightRemoveQueue[channel].pop();
    }

    propogateAddNodes(channel);
}
