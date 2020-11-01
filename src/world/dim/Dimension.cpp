//
// Created by aurailus on 2020-05-26.
//

#include "Dimension.h"

#include "world/dim/chunk/Chunk.h"
#include "game/Subgame.h"
#include "game/def/BlockDef.h"
#include "game/atlas/DefinitionAtlas.h"

bool Dimension::setBlock(glm::ivec3 pos, unsigned int block) {
    auto l = getReadLock();
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    l.unlock();

    if (!chunk) return false;

    if (!DimensionBase::setBlock(pos, block)) return false;

    auto &def = game->getDefs().blockFromId(block);

    glm::ivec4 oldLight = chunk->getLight(Space::Block::index(pos));
    glm::ivec3 newLight = def.lightSource;

    if (oldLight.x + oldLight.y + oldLight.z != 0) removeBlockLight(pos);
    if (newLight.x + newLight.y + newLight.z != 0) addBlockLight(pos, newLight);

    if (def.lightPropagates) reflowLight(pos);

    if (!def.lightPropagates && getLight(pos, chunk.get()).w != 0) removeSunlight(pos);

    propogateRemoveNodes();

    return true;
}

unsigned int Dimension::nextEntityInd() {
    auto l = getWriteLock();
    return entityInd++;
}

//std::unordered_set<glm::ivec3, Vec::ivec3> Dimension::calculateEdgeLight(glm::ivec3 mbPos) {
//    bool ypos = mapBlockGenerated(mbPos + glm::ivec3 {0, 1, 0});
//    bool yneg = mapBlockGenerated(mbPos + glm::ivec3 {0, -1, 0});
//    bool xpos = mapBlockGenerated(mbPos + glm::ivec3 {1, 0, 0});
//    bool xneg = mapBlockGenerated(mbPos + glm::ivec3 {-1, 0, 0});
//    bool zpos = mapBlockGenerated(mbPos + glm::ivec3 {0, 0, 1});
//    bool zneg = mapBlockGenerated(mbPos + glm::ivec3 {0, 0, -1});
//
//    for (unsigned int i = 0; i < 64; i++) {
//        glm::ivec3 l = Space::Chunk::fromIndex(i);
//        glm::ivec3 chunkPos = mbPos * 4 + l;
//
//        auto self = getChunk(chunkPos);
//
//        if (yneg && l.y == 0) calculateVerticalEdge(self, getChunk(chunkPos + glm::ivec3 {0, -1, 0}));
//        else if (ypos && l.y == 3) calculateVerticalEdge(getChunk(chunkPos + glm::ivec3 {0, 1, 0}), self);
//
//        if (xpos && l.x == 3) calculateHorizontalEdge(self, getChunk(chunkPos + glm::ivec3 {1, 0, 0}));
//        else if (xneg && l.x == 0) calculateHorizontalEdge(self, getChunk(chunkPos + glm::ivec3 {-1, 0, 0}));
//
//        if (zpos && l.z == 3) calculateHorizontalEdge(self, getChunk(chunkPos + glm::ivec3 {0, 0, 1}));
//        else if (zneg && l.z == 0) calculateHorizontalEdge(self, getChunk(chunkPos + glm::ivec3 {0, 0, -1}));
//    }
//
//    return propogateRemoveNodes();
//}

std::unordered_set<glm::ivec3, Vec::ivec3> Dimension::propogateAddNodes() {
    std::unordered_set<glm::ivec3, Vec::ivec3> chunksUpdated {};

    for (unsigned int channel = 0; channel < 4; channel++) {
        while (true) {
            auto l = getWriteLock();
            if (lightAddQueue[channel].empty()) break;
            LightAddNode node = lightAddQueue[channel].front();
            lightAddQueue[channel].pop();
            l.unlock();

            Chunk* chunk = node.chunk;
            if (!chunksUpdated.count(chunk->getPos())) chunksUpdated.insert(chunk->getPos());

            unsigned char lightLevel = node.chunk->getLight(node.index, channel);
            glm::ivec3 worldPos = node.chunk->getPos() * 16 + Space::Block::fromIndex(node.index);

            for (const auto& i : Vec::TO_VEC) {
                glm::ivec3 check = worldPos + i;
                unsigned int ind = Space::Block::index(check);
                Chunk* chunk;
                if (containsWorldPos(node.chunk, check)) chunk = node.chunk;
                else {
                    chunk = getChunk(Space::Chunk::world::fromBlock(check)).get();
                    if (!chunk) continue;
                    if (!chunksUpdated.count(chunk->getPos())) {
                        chunksUpdated.insert(chunk->getPos());
                        chunk->setDirty(true);
                    }
                }

                bool sunDown = (channel == SUNLIGHT_CHANNEL && lightLevel == 15 && i.y == -1);
                if (game->getDefs().blockFromId(chunk->getBlock(ind)).lightPropagates && (sunDown || chunk->getLight(ind, channel) + 2 <= lightLevel)) {
                    int subtract = sunDown ? 0 : 1;
                    chunk->setLight(ind, channel, lightLevel - subtract);

                    auto l = getWriteLock();
                    lightAddQueue[channel].emplace(ind, chunk);
                    l.unlock();
                }
            }
        }
    }

    return chunksUpdated;
}

std::unordered_set<glm::ivec3, Vec::ivec3> Dimension::propogateRemoveNodes() {
    std::unordered_set<glm::ivec3, Vec::ivec3> chunksUpdated {};

    for (unsigned int channel = 0; channel < 4; channel++) {
        while (true) {
            auto l = getWriteLock();
            if (lightRemoveQueue[channel].empty()) break;
            LightRemoveNode node = lightRemoveQueue[channel].front();
            lightRemoveQueue[channel].pop();
            l.unlock();

            glm::ivec3 worldPos = node.chunk->getPos() * 16 + Space::Block::fromIndex(node.index);

            for (const auto& i : Vec::TO_VEC) {
                glm::ivec3 check = worldPos + i;
                unsigned int ind = Space::Block::index(check);
                Chunk* chunk;
                if (containsWorldPos(node.chunk, check)) chunk = node.chunk;
                else {
                    chunk = getChunk(Space::Chunk::world::fromBlock(check)).get();
                    if (!chunk) continue;
                    if (!chunksUpdated.count(chunk->getPos())) {
                        chunksUpdated.insert(chunk->getPos());
                        chunk->setDirty(true);
                    }
                }

                unsigned char checkLight = chunk->getLight(ind, channel);
                if (checkLight != 0 && (checkLight < node.value || (channel == SUNLIGHT_CHANNEL && i.y == -1 && node.value == 15))) {
                    unsigned int replaceLight = (channel == SUNLIGHT_CHANNEL ? 0 :
                        game->getDefs().blockFromId(chunk->getBlock(Space::Block::index(check))).lightSource[channel]);
                    chunk->setLight(ind, channel, replaceLight);

                    auto l = getWriteLock();
                    if (replaceLight) lightAddQueue[channel].emplace(ind, chunk);
                    lightRemoveQueue[channel].emplace(ind, checkLight, chunk);
                    l.unlock();
                }
                else if (checkLight >= node.value) {
                    auto chunk = containsWorldPos(node.chunk, check) ? node.chunk : getChunk(Space::Chunk::world::fromBlock(check)).get();
                    if (!chunk) continue;

                    auto l = getWriteLock();
                    lightAddQueue[channel].emplace(ind, chunk);
                    l.unlock();
                }
            }
        }
    }

    auto otherChunksUpdated = propogateAddNodes();
    chunksUpdated.insert(otherChunksUpdated.begin(), otherChunksUpdated.end());

    return chunksUpdated;
}

bool Dimension::containsWorldPos(Chunk *chunk, glm::ivec3 pos) {
    return chunk && Space::Chunk::world::fromBlock(pos) == chunk->getPos();
}

glm::ivec4 Dimension::getLight(glm::ivec3 worldPos, Chunk *chunk) {
    if (containsWorldPos(chunk, worldPos)) return chunk->getLight(Space::Block::index(worldPos));
    auto oChunk = getChunk(Space::Chunk::world::fromBlock(worldPos)).get();
    return (oChunk ? oChunk->getLight(Space::Block::index(worldPos)) : glm::ivec4 {});
}

void Dimension::calculateHorizontalEdge(std::shared_ptr<Chunk> a, std::shared_ptr<Chunk> b) {
    for (unsigned int j = 0; j < 256; j++) {
        glm::ivec3 diff = a->getPos() - b->getPos();

        glm::ivec3 aPos = {
            (diff.x == 0 ? j % 16 : diff.x == 1 ? 0 : 15), j / 16,
            (diff.z == 0 ? j % 16 : diff.z == 1 ? 0 : 15) };
        glm::ivec3 bPos = {
            (diff.x == 0 ? j % 16 : diff.x == 1 ? 15 : 0), j / 16,
            (diff.z == 0 ? j % 16 : diff.z == 1 ? 15 : 0) };

        auto lightA = a->getLight(Space::Block::index(aPos), 3);
        auto lightB = b->getLight(Space::Block::index(bPos), 3);

        if (lightA > lightB + 1) setAndReflowSunlight(b->getPos() * 16 + bPos, lightA - 1);
        else if (lightB > lightA + 1) setAndReflowSunlight(a->getPos() * 16 + aPos, lightB - 1);
    }
}

void Dimension::calculateVerticalEdge(std::shared_ptr<Chunk> above, std::shared_ptr<Chunk> below) {
    for (unsigned int j = 0; j < 256; j++) {
        unsigned int xx = j / 16;
        unsigned int zz = j % 16;

        auto lightAbove = above->getLight(Space::Block::index({xx, 0, zz}), 3);
        auto lightBelow = below->getLight(Space::Block::index({xx, 15, zz}), 3);

        if (lightBelow > lightAbove) removeSunlight(below->getPos() * 16 + glm::ivec3{xx, 15, zz});
    }
}

void Dimension::addBlockLight(glm::ivec3 pos, glm::ivec3 light) {
    auto startChunk = getChunk(Space::Chunk::world::fromBlock(pos));
    auto ind = Space::Block::index(pos);

    startChunk->setLight(ind, 0, light.x);
    startChunk->setLight(ind, 1, light.y);
    startChunk->setLight(ind, 2, light.z);

    auto l = getWriteLock();
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

    auto l = getWriteLock();
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

    auto l = getWriteLock();
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
    auto l = getWriteLock();
    lightRemoveQueue[SUNLIGHT_CHANNEL].emplace(ind, light, chunk.get());
}

void Dimension::setAndReflowSunlight(glm::ivec3 pos, unsigned char level) {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    unsigned int ind = Space::Block::index(pos);

    chunk->setLight(ind, 3, level);
    auto l = getWriteLock();
    lightAddQueue[SUNLIGHT_CHANNEL].emplace(ind, chunk.get());
}