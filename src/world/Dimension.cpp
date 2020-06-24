//
// Created by aurailus on 2020-05-26.
//

#include "Dimension.h"

#include "chunk/Chunk.h"

bool Dimension::setBlock(glm::ivec3 pos, unsigned int block) {
    if (!DimensionBase::setBlock(pos, block)) return false;

    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    auto &def = defs.blockFromId(block);

    glm::ivec4 oldLight = chunk->getLight(Space::Block::index(pos));
    glm::ivec3 newLight = def.lightSource;

    if (oldLight.x + oldLight.y + oldLight.z != 0) removeBlockLight(pos);
    if (newLight.x + newLight.y + newLight.z != 0) addBlockLight(pos, newLight);

    if (def.lightPropagates) reflowLight(pos);
    if (!def.lightPropagates && getLight(pos, chunk.get()).w != 0) removeSunlight(pos);

    propogateRemoveNodes();

    return true;
}

std::unordered_set<glm::ivec3, Vec::ivec3> Dimension::calculateEdgeLight(glm::ivec3 mbPos) {
    bool ypos = mapBlockGenerated(mbPos + glm::ivec3 {0, 1, 0});
    bool yneg = mapBlockGenerated(mbPos + glm::ivec3 {0, -1, 0});
    bool xpos = mapBlockGenerated(mbPos + glm::ivec3 {1, 0, 0});
    bool xneg = mapBlockGenerated(mbPos + glm::ivec3 {-1, 0, 0});
    bool zpos = mapBlockGenerated(mbPos + glm::ivec3 {0, 0, 1});
    bool zneg = mapBlockGenerated(mbPos + glm::ivec3 {0, 0, -1});

    for (unsigned int i = 0; i < 64; i++) {
        glm::ivec3 l = Space::Chunk::fromIndex(i);
        glm::ivec3 chunkPos = mbPos * 4 + l;

        auto self = getChunk(chunkPos);

        if (yneg && l.y == 0) calculateVerticalEdge(self, getChunk(chunkPos + glm::ivec3 {0, -1, 0}));
        else if (ypos && l.y == 3) calculateVerticalEdge(getChunk(chunkPos + glm::ivec3 {0, 1, 0}), self);

        if (xpos && l.x == 3) calculateHorizontalEdge(self, getChunk(chunkPos + glm::ivec3 {1, 0, 0}));
        if (xneg && l.x == 0) calculateHorizontalEdge(self, getChunk(chunkPos + glm::ivec3 {-1, 0, 0}));

        if (zpos && l.z == 3) calculateHorizontalEdge(self, getChunk(chunkPos + glm::ivec3 {0, 0, 1}));
        if (zneg && l.z == 0) calculateHorizontalEdge(self, getChunk(chunkPos + glm::ivec3 {0, 0, -1}));
    }

    return propogateRemoveNodes();
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
                Chunk* chunk;
                if (containsWorldPos(node.chunk, check)) chunk = node.chunk;
                else {
                    chunk = getChunk(Space::Chunk::world::fromBlock(check)).get();
                    if (!chunk) continue;
                    chunksUpdated.insert(chunk->pos);
                    chunk->dirty = true;
                }

                bool sunDown = (channel == SUNLIGHT_CHANNEL && lightLevel == 15 && i.y == -1);
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
                Chunk* chunk;
                if (containsWorldPos(node.chunk, check)) chunk = node.chunk;
                else {
                    chunk = getChunk(Space::Chunk::world::fromBlock(check)).get();
                    if (!chunk) continue;
                    chunksUpdated.insert(chunk->pos);
                    chunk->dirty = true;
                }

                unsigned char checkLight = chunk->getLight(ind, channel);
                if (checkLight != 0 && (checkLight < node.value || (channel == SUNLIGHT_CHANNEL && i.y == -1 && node.value == 15))) {
                    unsigned int replaceLight = (channel == SUNLIGHT_CHANNEL ? 0 :
                        defs.blockFromId(chunk->getBlock(Space::Block::index(check))).lightSource[channel]);
                    chunk->setLight(ind, channel, replaceLight);

                    if (replaceLight) lightAddQueue[channel].emplace(ind, chunk);
                    lightRemoveQueue[channel].emplace(ind, checkLight, chunk);
                }
                else if (checkLight >= node.value) {
                    auto chunk = containsWorldPos(node.chunk, check) ? node.chunk : getChunk(Space::Chunk::world::fromBlock(check)).get();
                    if (!chunk) continue;
                    lightAddQueue[channel].emplace(ind, chunk);
                }
            }

            lightRemoveQueue[channel].pop();
        }
    }

    auto otherChunksUpdated = propogateAddNodes();
    chunksUpdated.insert(otherChunksUpdated.begin(), otherChunksUpdated.end());

    return chunksUpdated;
}

bool Dimension::containsWorldPos(Chunk *chunk, glm::ivec3 pos) {
    return chunk && Space::Chunk::world::fromBlock(pos) == chunk->pos;
}

glm::ivec4 Dimension::getLight(glm::ivec3 worldPos, Chunk *chunk) {
    if (containsWorldPos(chunk, worldPos)) return chunk->getLight(Space::Block::index(worldPos));
    auto oChunk = getChunk(Space::Chunk::world::fromBlock(worldPos)).get();
    return (oChunk ? oChunk->getLight(Space::Block::index(worldPos)) : glm::ivec4 {});
}

void Dimension::calculateHorizontalEdge(std::shared_ptr<Chunk> a, std::shared_ptr<Chunk> b) {
    for (unsigned int j = 0; j < 256; j++) {
        glm::ivec3 diff = a->pos - b->pos;

        glm::ivec3 aPos = {
            (diff.x == 0 ? j % 16 : diff.x == 1 ? 0 : 15), j / 16,
            (diff.z == 0 ? j % 16 : diff.z == 1 ? 0 : 15) };
        glm::ivec3 bPos = {
            (diff.x == 0 ? j % 16 : diff.x == 1 ? 15 : 0), j / 16,
            (diff.z == 0 ? j % 16 : diff.z == 1 ? 15 : 0) };

        auto lightA = a->getLight(Space::Block::index(aPos), 3);
        auto lightB = b->getLight(Space::Block::index(bPos), 3);

        if (lightA > lightB + 1) setAndReflowSunlight(b->pos * 16 + bPos, lightA - 1);
        else if (lightB > lightA + 1) setAndReflowSunlight(a->pos * 16 + aPos, lightB - 1);
    }
}

void Dimension::calculateVerticalEdge(std::shared_ptr<Chunk> above, std::shared_ptr<Chunk> below) {
    for (unsigned int j = 0; j < 256; j++) {
        unsigned int xx = j / 16;
        unsigned int zz = j % 16;

        auto lightAbove = above->getLight(Space::Block::index({xx, 0, zz}), 3);
        auto lightBelow = below->getLight(Space::Block::index({xx, 15, zz}), 3);

        if (lightBelow > lightAbove) removeSunlight(below->pos * 16 + glm::ivec3{xx, 15, zz});
    }
}

void Dimension::addBlockLight(glm::ivec3 pos, glm::ivec3 light) {
    auto startChunk = getChunk(Space::Chunk::world::fromBlock(pos));
    auto ind = Space::Block::index(pos);

    startChunk->setLight(ind, 0, light.x);
    startChunk->setLight(ind, 1, light.y);
    startChunk->setLight(ind, 2, light.z);

    lightAddQueue[0].emplace(ind, startChunk.get());
    lightAddQueue[1].emplace(ind, startChunk.get());
    lightAddQueue[2].emplace(ind, startChunk.get());
}

void Dimension::removeBlockLight(glm::ivec3 pos) {
    auto startChunk = getChunk(Space::Chunk::world::fromBlock(pos));
    unsigned int ind = Space::Block::index(pos);
    glm::ivec4 val = startChunk->getLight(ind);

    startChunk->setLight(ind, 0, 0);
    startChunk->setLight(ind, 1, 0);
    startChunk->setLight(ind, 2, 0);

    lightRemoveQueue[0].emplace(ind, val.x, startChunk.get());
    lightRemoveQueue[1].emplace(ind, val.y, startChunk.get());
    lightRemoveQueue[2].emplace(ind, val.z, startChunk.get());
}

void Dimension::reflowLight(glm::ivec3 pos) {
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

    chunk->setLight(ind, placeLight);

    lightAddQueue[0].emplace(ind, chunk.get());
    lightAddQueue[1].emplace(ind, chunk.get());
    lightAddQueue[2].emplace(ind, chunk.get());
    lightAddQueue[3].emplace(ind, chunk.get());
}

void Dimension::removeSunlight(glm::ivec3 pos) {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    unsigned int ind = Space::Block::index(pos);
    unsigned int light = chunk->getLight(ind, 3);

    chunk->setLight(ind, 3, 0);
    lightRemoveQueue[SUNLIGHT_CHANNEL].emplace(ind, light, chunk.get());
}

void Dimension::setAndReflowSunlight(glm::ivec3 pos, unsigned char level) {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    unsigned int ind = Space::Block::index(pos);

    chunk->setLight(ind, 3, level);
    lightAddQueue[SUNLIGHT_CHANNEL].emplace(ind, chunk.get());
}