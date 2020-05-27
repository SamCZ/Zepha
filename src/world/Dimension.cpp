//
// Created by aurailus on 2020-05-26.
//

#include "Dimension.h"

void Dimension::calculateEdgeLight(glm::ivec3 mbPos) {
    for (auto i = 0; i < 64; i++) {
        glm::ivec3 l = Space::Chunk::fromIndex(i);
        glm::ivec3 chunkPos = mbPos * 4 + l;

        auto edges = std::array<bool, 6> { l.x == 0, l.y == 0, l.z == 0,
                                           l.x == Space::MAPBLOCK_SIZE, l.y == Space::MAPBLOCK_SIZE, l.z == Space::MAPBLOCK_SIZE};

        if (edges[1]) {
            for (unsigned int j = 0; j < 64; j++) {
                for (unsigned int k = 0; k < 64; k++) {

                }
            }
        }
    }
}

void Dimension::reflowSunlightAroundSolid(glm::ivec3 pos) {
    auto startChunk = getChunk(Space::Chunk::world::fromBlock(pos));
    auto ind = Space::Block::index(pos);
    unsigned int light = startChunk->getSunlight(ind);
    startChunk->setSunlight(ind, 0);
    lightRemoveQueue[SUNLIGHT_CHANNEL].emplace(ind, light, startChunk.get());
    propogateRemoveNodes();
}

bool Dimension::containsWorldPos(BlockChunk *chunk, glm::ivec3 pos) {
    return chunk && Space::Chunk::world::fromBlock(pos) == chunk->pos;
}

glm::ivec4 Dimension::getLight(glm::ivec3 worldPos, BlockChunk *chunk) {
    if (containsWorldPos(chunk, worldPos)) return chunk->getLight(Space::Block::index(worldPos));
    auto oChunk = getChunk(Space::Chunk::world::fromBlock(worldPos)).get();
    return (oChunk ? oChunk->getLight(Space::Block::index(worldPos)) : glm::ivec4 {});
}

void Dimension::addBlockLight(glm::ivec3 pos, glm::ivec3 light) {
    auto startChunk = getChunk(Space::Chunk::world::fromBlock(pos));
    auto ind = Space::Block::index(pos);

    startChunk->setBlockLight(ind, light);
    lightAddQueue[0].emplace(ind, startChunk.get());
    lightAddQueue[1].emplace(ind, startChunk.get());
    lightAddQueue[2].emplace(ind, startChunk.get());
    propogateAddNodes();
}

void Dimension::removeBlockLight(glm::ivec3 pos) {
    auto startChunk = getChunk(Space::Chunk::world::fromBlock(pos));
    auto ind = Space::Block::index(pos);

    glm::ivec4 val = startChunk->getLight(ind);

    startChunk->setBlockLight(Space::Block::index(pos), {});
    lightRemoveQueue[0].emplace(ind, val.x, startChunk.get());
    lightRemoveQueue[1].emplace(ind, val.y, startChunk.get());
    lightRemoveQueue[2].emplace(ind, val.z, startChunk.get());
    propogateRemoveNodes();
}

void Dimension::reflowLightAroundTransparent(glm::ivec3 pos) {
    glm::ivec4 placeLight {};
    const static std::array<glm::ivec3, 6> checks = { glm::ivec3 {-1, 0, 0}, {1, 0, 0}, {0, -1, 0}, {0, 1, 0}, {0, 0, -1}, {0, 0, 1} };

    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    auto ind = Space::Block::index(pos);
    if (!chunk) return;

    for (const auto& i : checks) {
        glm::ivec3 check = pos + i;
        auto adjLight = getLight(check, chunk.get());
        placeLight.x = fmax(placeLight.x, adjLight.x - 1);
        placeLight.y = fmax(placeLight.y, adjLight.y - 1);
        placeLight.z = fmax(placeLight.z, adjLight.z - 1);
        placeLight.w = fmax(placeLight.w, adjLight.w - (i.y == 1 ? 0 : 1));
    }

    chunk->setBlockLight(ind, {placeLight.x, placeLight.y, placeLight.z});
    chunk->setSunlight(ind, placeLight.w);

    lightAddQueue[0].emplace(ind, chunk.get());
    lightAddQueue[1].emplace(ind, chunk.get());
    lightAddQueue[2].emplace(ind, chunk.get());
    lightAddQueue[3].emplace(ind, chunk.get());

    propogateAddNodes();
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

bool Dimension::setBlock(glm::ivec3 pos, unsigned int block) {
    if (!DimensionBase::setBlock(pos, block)) return false;

    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    auto &def = defs.blockFromId(block);

    // Remove light when placing solid blocks.
    glm::ivec4 oldLight = chunk->getLight(Space::Block::index(pos));
    if (!def.lightPropagates && oldLight.x + oldLight.y + oldLight.z != 0) removeBlockLight(pos);

    // Add light when placing light emitting blocks.
    glm::ivec3 newLight = def.lightSource;
    if (newLight.x > oldLight.x || newLight.y > oldLight.y || newLight.z > oldLight.z) addBlockLight(pos, newLight);

    // Reflow light when a transparent block is placed.
    if (def.lightPropagates) reflowLightAroundTransparent(pos);

    // Block sunlight when a solid block is placed.
    if (!def.lightPropagates && getLight(pos, chunk.get()).w != 0) reflowSunlightAroundSolid(pos);

    return true;
}
