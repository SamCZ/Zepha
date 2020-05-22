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

    glm::ivec4 oldLight = chunk->getLight(Space::Block::index(pos));
    glm::ivec3 newLight = defs.blockFromId(block).lightSource;
    if (oldLight.x != newLight.x || oldLight.y != newLight.y || oldLight.z != newLight.z) {
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

//
// Light related functions.
//

void Dimension::createSunlight(glm::ivec3 pos) {
//    auto chunk = getChunk(pos);
//    auto top = getChunk(pos + glm::ivec3 {0, 1, 0});
//    if (top) {
//        for (unsigned int i = 0; i < 256; i++) {
//            unsigned int ind = Space::Block::index(glm::ivec3 {i / 16, 0, i % 16});
//            auto light = top->getSunlight(ind);
//            if (light != 0) {
//                lightAddQueue[SUNLIGHT_CHANNEL].emplace(ind, top.get());
//                if (light == 15) for (int j = 15; j >= 0; j--) {
//                    unsigned int ind = Space::Block::index({i / 16, j, i % 16});
//                    if (!defs.blockFromId(chunk->getBlock(ind)).solid) {
//                        chunk->setSunlight(ind, 15);
//                        lightAddQueue[SUNLIGHT_CHANNEL].emplace(ind, chunk.get());
//                    }
//                }
//            }
//        }
//    }
//    else {
//        for (unsigned int i = 0; i < 256; i++) {
//            for (unsigned int j = 15; j >= 0; j--) {
//                auto index = Space::Block::index(glm::ivec3{i / 16, j, i % 16});
//                if (defs.blockFromId(chunk->getBlock(index)).solid) continue;
//                chunk->setSunlight(index, 15);
//                lightAddQueue[SUNLIGHT_CHANNEL].emplace(index, chunk.get());
//            }
//        }
//    }
}


// Returns true if the provided pos references a block within chunk, otherwise returns false.
bool Dimension::containsWorldPos(BlockChunk *chunk, glm::ivec3 pos) {
    return chunk && Space::Chunk::world::fromBlock(pos) == chunk->pos;
}

// Get the BlockLight of a block. This function can be accelerated
// by providing a chunk that might contain the world position.
glm::ivec4 Dimension::getLight(glm::ivec3 worldPos, BlockChunk *chunk) {
    if (containsWorldPos(chunk, worldPos)) return chunk->getLight(Space::Block::index(worldPos));
    auto oChunk = getChunk(Space::Chunk::world::fromBlock(worldPos)).get();
    return (oChunk ? oChunk->getLight(Space::Block::index(worldPos)) : glm::ivec4 {});
}

void Dimension::addLight(glm::ivec3 pos, glm::ivec3 light) {
    auto startChunk = getChunk(Space::Chunk::world::fromBlock(pos));
    auto index = Space::Block::index(pos);

    startChunk->setBlockLight(index, light);
    lightAddQueue[0].emplace(index, startChunk.get());
    lightAddQueue[1].emplace(index, startChunk.get());
    lightAddQueue[2].emplace(index, startChunk.get());
    propogateAddNodes();
}

void Dimension::removeLight(glm::ivec3 pos) {
    auto startChunk = getChunk(Space::Chunk::world::fromBlock(pos));
    glm::ivec4 val = startChunk->getLight(Space::Block::index(pos));

    startChunk->setBlockLight(Space::Block::index(pos), {});
    lightRemoveQueue[0].emplace(Space::Block::index(pos), val.x, startChunk.get());
    lightRemoveQueue[1].emplace(Space::Block::index(pos), val.y, startChunk.get());
    lightRemoveQueue[2].emplace(Space::Block::index(pos), val.z, startChunk.get());
    propogateRemoveNodes();
}

std::unordered_set<glm::ivec3, Vec::ivec3> Dimension::propogateAddNodes() {
    std::unordered_set<glm::ivec3, Vec::ivec3> chunksUpdated {};

    for (unsigned int channel = 0; channel < SUNLIGHT_CHANNEL; channel++) {
        while (!lightAddQueue[channel].empty()) {
            LightAddNode& node = lightAddQueue[channel].front();

            unsigned char lightLevel = node.chunk->getLight(node.index, channel);
            glm::ivec3 worldPos = node.chunk->pos * 16 + Space::Block::fromIndex(node.index);

            const static std::array<glm::ivec3, 6> checks = { glm::ivec3 {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1} };
            for (const auto& i : checks) {
                glm::ivec3 check = worldPos + i;
                if (defs.blockFromId(getBlock(check)).lightPropagates && getLight(check, node.chunk)[channel] + 2 <= lightLevel) {
                    BlockChunk* chunk;
                    if (containsWorldPos(node.chunk, check)) chunk = node.chunk;
                    else {
                        chunk = getChunk(Space::Chunk::world::fromBlock(check)).get();
                        if (!chunk) continue;
                        chunksUpdated.insert(chunk->pos);
                        chunk->dirty = true;
                    }

                    chunk->setLight(Space::Block::index(check), channel, lightLevel - 1);
                    lightAddQueue[channel].emplace(Space::Block::index(check), chunk);
                }
            }
            lightAddQueue[channel].pop();
        }
    }

    while (!lightAddQueue[SUNLIGHT_CHANNEL].empty()) {
        LightAddNode& node = lightAddQueue[SUNLIGHT_CHANNEL].front();

        unsigned char lightLevel = node.chunk->getSunlight(node.index);
        glm::ivec3 worldPos = node.chunk->pos * 16 + Space::Block::fromIndex(node.index);

        const static std::array<glm::ivec3, 6> checks = { glm::ivec3 {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1} };
        for (const auto& i : checks) {
            glm::ivec3 check = worldPos + i;
            if (defs.blockFromId(getBlock(check)).lightPropagates && getLight(check, node.chunk)[SUNLIGHT_CHANNEL] + 2 <= lightLevel) {
                BlockChunk* chunk;
                if (containsWorldPos(node.chunk, check)) chunk = node.chunk;
                else {
                    chunk = getChunk(Space::Chunk::world::fromBlock(check)).get();
                    if (!chunk) continue;
                    chunksUpdated.insert(chunk->pos);
                    chunk->dirty = true;
                }

                int subtract = 1;
                if (lightLevel == 15 && i == checks[2]) subtract = 0;
                chunk->setLight(Space::Block::index(check), SUNLIGHT_CHANNEL, lightLevel - subtract);
                lightAddQueue[SUNLIGHT_CHANNEL].emplace(Space::Block::index(check), chunk);
            }
        }
        lightAddQueue[SUNLIGHT_CHANNEL].pop();
    }

    return chunksUpdated;
}

std::unordered_set<glm::ivec3, Vec::ivec3> Dimension::propogateRemoveNodes() {
    std::unordered_set<glm::ivec3, Vec::ivec3> chunksUpdated {};

    for (unsigned int channel = 0; channel < SUNLIGHT_CHANNEL; channel++) {
        while (!lightRemoveQueue[channel].empty()) {
            LightRemoveNode& node = lightRemoveQueue[channel].front();

            glm::ivec3 worldPos = node.chunk->pos * 16 + Space::Block::fromIndex(node.index);

            const static std::array<glm::ivec3, 6> checks = { glm::ivec3 {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1} };
            for (const auto& i : checks) {
                glm::ivec3 check = worldPos + i;
                unsigned char checkLight = getLight(check, node.chunk)[channel];

                if (checkLight != 0 && checkLight < node.value) {
                    BlockChunk* chunk;
                    if (containsWorldPos(node.chunk, check)) chunk = node.chunk;
                    else {
                        chunk = getChunk(Space::Chunk::world::fromBlock(check)).get();
                        if (!chunk) continue;
                        chunksUpdated.insert(chunk->pos);
                        chunk->dirty = true;
                    }

                    auto blockLight = defs.blockFromId(chunk->getBlock(Space::Block::index(check))).lightSource[channel];
                    chunk->setLight(Space::Block::index(check), channel, blockLight);
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
    }

    propogateAddNodes();
    return chunksUpdated;
}

void Dimension::propogateLight() {
    propogateRemoveNodes();
    propogateAddNodes();
}
