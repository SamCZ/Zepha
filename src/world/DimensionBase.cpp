//
// Created by aurailus on 2019-11-20.
//

#include "DimensionBase.h"

#include "chunk/Chunk.h"
#include "chunk/Region.h"
#include "chunk/MapBlock.h"
#include "../def/Subgame.h"
#include "../def/DefinitionAtlas.h"

DimensionBase::DimensionBase(Subgame &game, World& world, const std::string &identifier, unsigned int ind) :
    game(game), world(world), identifier(identifier), ind(ind) {}

std::string DimensionBase::getIdentifier() const {
    return identifier;
}

std::shared_ptr<Region> DimensionBase::getRegion(glm::ivec3 regionPosition) {
    if (!regions.count(regionPosition)) return nullptr;
    return regions[regionPosition];
}

void DimensionBase::removeRegion(glm::ivec3 pos) {
    regions.erase(pos);
}

std::shared_ptr<MapBlock> DimensionBase::getMapBlock(glm::ivec3 mapBlockPosition) {
    auto region = getRegion(Space::Region::world::fromMapBlock(mapBlockPosition));
    if (!region) return nullptr;
    return (*region)[Space::MapBlock::index(mapBlockPosition)];
}

void DimensionBase::removeMapBlock(glm::ivec3 pos) {
    auto region = getRegion(Space::Region::world::fromMapBlock(pos));
    if (!region) return;
    auto ind = Space::MapBlock::index(pos);
    region->remove(ind);
    if (region->count == 0) removeRegion(Space::Region::world::fromMapBlock(pos));
}

bool DimensionBase::mapBlockGenerated(glm::ivec3 mapBlockPosition) {
    auto mb = getMapBlock(mapBlockPosition);
    return mb && mb->generated;
}

std::shared_ptr<Chunk> DimensionBase::getChunk(glm::ivec3 chunkPosition) {
    auto mapBlock = getMapBlock(Space::MapBlock::world::fromChunk(chunkPosition));
    if (!mapBlock) return nullptr;
    return (*mapBlock)[Space::Chunk::index(chunkPosition)];
}

void DimensionBase::setChunk(std::shared_ptr<Chunk> chunk) {
    auto mapBlock = getOrCreateMapBlock(Space::MapBlock::world::fromChunk(chunk->pos));
    mapBlock->set(Space::Chunk::index(chunk->pos), chunk);
}

void DimensionBase::removeChunk(glm::ivec3 pos){
    auto mapBlock = getMapBlock(Space::MapBlock::world::fromChunk(pos));
    if (!mapBlock) return;
    auto ind = Space::Chunk::index(pos);
    mapBlock->remove(ind);
    if (mapBlock->count == 0) removeMapBlock(Space::MapBlock::world::fromChunk(pos));
}

unsigned int DimensionBase::getBlock(glm::ivec3 pos) {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    if (!chunk) return 0;
    auto l = chunk->aquireLock();

    return chunk->getBlock(Space::Block::relative::toChunk(pos));
}

bool DimensionBase::setBlock(glm::ivec3 pos, unsigned int block) {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    if (!chunk) return false;
    auto l = chunk->aquireLock();

    return chunk->setBlock(Space::Block::relative::toChunk(pos), block);
}

unsigned int DimensionBase::getBiome(glm::ivec3 pos) {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    if (!chunk) return 0;
    auto l = chunk->aquireLock();

    return chunk->getBiome(Space::Block::relative::toChunk(pos));
}

bool DimensionBase::setBiome(glm::ivec3 pos, unsigned int biome) {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    if (!chunk) return false;
    auto l = chunk->aquireLock();

    return chunk->setBiome(Space::Block::relative::toChunk(pos), biome);
}

std::shared_ptr<Region> DimensionBase::getOrCreateRegion(glm::ivec3 pos) {
    if (regions[pos]) return regions[pos];
    regions[pos] = std::make_shared<Region>(pos);
    return regions[pos];
}

std::shared_ptr<MapBlock> DimensionBase::getOrCreateMapBlock(glm::ivec3 mapBlockPosition) {
    auto region = getOrCreateRegion(Space::Region::world::fromMapBlock(mapBlockPosition));
    unsigned int index = Space::MapBlock::index(mapBlockPosition);

    if ((*region)[index] != nullptr) return (*region)[index];
    region->set(index, std::make_shared<MapBlock>(mapBlockPosition));
    return (*region)[index];
}

Subgame &DimensionBase::getGame() {
    return game;
}

World &DimensionBase::getWorld() {
    return world;
}

std::shared_ptr<Chunk> DimensionBase::combinePartials(std::shared_ptr<Chunk> a, std::shared_ptr<Chunk> b) {
    std::shared_ptr<Chunk> src;
    std::shared_ptr<Chunk> res;

    if (a->generated) {
        res = a;
        src = b;
    }
    else {
        res = b;
        src = a;
    }

    for (unsigned int i = 0; i < 4096; i++) {
        if (src->getBlock(i) > DefinitionAtlas::INVALID) res->setBlock(i, src->getBlock(i));
    }

    res->generated = src->generated || res->generated;
    res->partial = !res->generated;
    res->countRenderableBlocks();
    return res;
}

double DimensionBase::getBlockDamage(glm::ivec3 pos) const {
    return 0;
}

double DimensionBase::setBlockDamage(glm::ivec3 pos, double damage) {
    return 0;
    //TODO: WOwdowadoawod
//    double totalDamage = World::setBlockDamage(pos, damage);

//    BlockCrackEntity* block = nullptr;
//    if (crackEntities.count(pos)) block = crackEntities[pos];
//    else block = new BlockCrackEntity(game.defs->blockFromId(getBlock(pos)), game.textures, pos);
//    block->setDamage(damage);
//    block->time = 0;
//
//    return totalDamage;
}

unsigned int DimensionBase::getInd() {
    return ind;
}
