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

    // Remove light when placing solid blocks.
    glm::ivec4 oldLight = chunk->getLight(Space::Block::index(pos));
    if (!def.lightPropagates && oldLight.x + oldLight.y + oldLight.z != 0) removeLight(pos);

    // Add light when placing light emitting blocks.
    glm::ivec3 newLight = def.lightSource;
    if (newLight.x > oldLight.x || newLight.y > oldLight.y || newLight.z > oldLight.z) addLight(pos, newLight);

    // Reflow light when a transparent block is placed.
    if (def.lightPropagates) reflowLight(pos);

    // Block sunlight when a solid block is placed.
    if (!def.lightPropagates && getLight(pos, chunk.get()).w != 0) blockSunlight(pos);

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

void Dimension::blockSunlight(glm::ivec3 pos) {
    auto startChunk = getChunk(Space::Chunk::world::fromBlock(pos));
    auto ind = Space::Block::index(pos);
    unsigned int light = startChunk->getSunlight(ind);
    startChunk->setSunlight(ind, 0);
    lightRemoveQueue[SUNLIGHT_CHANNEL].emplace(ind, light, startChunk.get());
    propogateRemoveNodes();
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

    for (unsigned int channel = 0; channel < lightAddQueue.size(); channel++) {
        while (!lightAddQueue[channel].empty()) {
            LightAddNode& node = lightAddQueue[channel].front();
            unsigned char lightLevel = node.chunk->getLight(node.index, channel);
            glm::ivec3 worldPos = node.chunk->pos * 16 + Space::Block::fromIndex(node.index);

            for (const auto& i : Vec::adj) {
                glm::ivec3 check = worldPos + i;
                unsigned int ind = Space::Block::index(check);
                BlockChunk* chunk;
                if (containsWorldPos(node.chunk, check)) chunk = node.chunk;
                else {
                    chunk = getChunk(Space::Chunk::world::fromBlock(check)).get();
                    if (!chunk) continue;
                    chunksUpdated.insert(chunk->pos);
                    chunk->dirty = true;
                }

                bool sunDown = channel == SUNLIGHT_CHANNEL && lightLevel == 15 && i.y == -1;
                if (defs.blockFromId(chunk->getBlock(ind)).lightPropagates && (sunDown || chunk->getLight(ind, channel) + 2 <= lightLevel)) {
                    int subtract = sunDown ? 0 : 1;
                    chunk->setLight(ind, channel, lightLevel - subtract);
                    lightAddQueue[channel].emplace(ind, chunk);
                }
            }

            lightAddQueue[channel].pop();
        }
    }

    return chunksUpdated;
}

std::unordered_set<glm::ivec3, Vec::ivec3> Dimension::propogateRemoveNodes() {
    std::unordered_set<glm::ivec3, Vec::ivec3> chunksUpdated {};

    for (unsigned int channel = 0; channel < lightRemoveQueue.size(); channel++) {
        while (!lightRemoveQueue[channel].empty()) {
            LightRemoveNode& node = lightRemoveQueue[channel].front();
            glm::ivec3 worldPos = node.chunk->pos * 16 + Space::Block::fromIndex(node.index);

            for (const auto& i : Vec::adj) {
                glm::ivec3 check = worldPos + i;
                unsigned int ind = Space::Block::index(check);
                BlockChunk* chunk;
                if (containsWorldPos(node.chunk, check)) chunk = node.chunk;
                else {
                    chunk = getChunk(Space::Chunk::world::fromBlock(check)).get();
                    if (!chunk) continue;
                    chunksUpdated.insert(chunk->pos);
                    chunk->dirty = true;
                }

                unsigned char checkLight = chunk->getLight(ind, channel);
                if (checkLight != 0 && (checkLight < node.value || (i.y == -1 && node.value == 15))) {
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

    propogateAddNodes(); //TODO: Merge returned chunks with our list
    return chunksUpdated;
}

void Dimension::propogateLight() {
    propogateRemoveNodes();
    propogateAddNodes();
}
