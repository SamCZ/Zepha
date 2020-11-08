//
// Created by aurailus on 2019-11-20.
//

#include "DimensionBase.h"

#include "game/Subgame.h"
#include "world/gen/MapGen.h"
#include "game/def/BlockDef.h"
#include "world/dim/chunk/Chunk.h"
#include "world/dim/chunk/Region.h"
#include "world/dim/chunk/MapBlock.h"
#include "game/atlas/DefinitionAtlas.h"

DimensionBase::DimensionBase(SubgamePtr game, World& world, const std::string &identifier,
    unsigned int ind, std::shared_ptr<MapGen> mapGen) :
    game(game), world(world), identifier(identifier), ind(ind), mapGen(std::move(mapGen)) {}

std::string DimensionBase::getIdentifier() const {
    return identifier;
}

unsigned int DimensionBase::getInd() {
    return ind;
}

std::shared_ptr<Region> DimensionBase::getRegion(glm::ivec3 regionPosition) const {
    auto _ = getReadLock();
    if (!regions.count(regionPosition)) return nullptr;
    return regions.at(regionPosition);
}

void DimensionBase::removeRegion(glm::ivec3 pos) {
    auto _ = getWriteLock();
    regions.erase(pos);
}

std::shared_ptr<MapBlock> DimensionBase::getMapBlock(glm::ivec3 mapBlockPosition) const {
    auto region = getRegion(Space::Region::world::fromMapBlock(mapBlockPosition));
    if (!region) return nullptr;
    return region->get(Space::MapBlock::index(mapBlockPosition));
}

void DimensionBase::removeMapBlock(glm::ivec3 pos) {
    auto region = getRegion(Space::Region::world::fromMapBlock(pos));
    if (!region) return;
    auto i = Space::MapBlock::index(pos);
    region->remove(i);
    if (region->count == 0) removeRegion(Space::Region::world::fromMapBlock(pos));
}

bool DimensionBase::mapBlockGenerated(glm::ivec3 mapBlockPosition) {
    auto mb = getMapBlock(mapBlockPosition);
    return mb && mb->generated;
}

std::shared_ptr<Chunk> DimensionBase::getChunk(glm::ivec3 chunkPosition) const {
    auto mapBlock = getMapBlock(Space::MapBlock::world::fromChunk(chunkPosition));
    if (!mapBlock) return nullptr;
    return mapBlock->get(Space::Chunk::index(chunkPosition));
}

void DimensionBase::setChunk(std::shared_ptr<Chunk> chunk) {
    auto mapBlock = getOrCreateMapBlock(Space::MapBlock::world::fromChunk(chunk->getPos()));
    mapBlock->set(Space::Chunk::index(chunk->getPos()), chunk);
}

void DimensionBase::removeChunk(glm::ivec3 pos){
    auto mapBlock = getMapBlock(Space::MapBlock::world::fromChunk(pos));
    if (!mapBlock) return;
    auto i = Space::Chunk::index(pos);
    mapBlock->remove(i);
    if (mapBlock->count == 0) removeMapBlock(Space::MapBlock::world::fromChunk(pos));
}

unsigned int DimensionBase::getBlock(glm::ivec3 pos) const {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    if (!chunk) return 0;

    return chunk->getBlock(Space::Block::relative::toChunk(pos));
}

bool DimensionBase::setBlock(glm::ivec3 pos, unsigned int block) {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    if (!chunk) return false;

    bool manip = chunk->setBlock(Space::Block::relative::toChunk(pos), block);
    setBlockDamage(pos, 0);
    return manip;
}


double DimensionBase::getBlockDamage(glm::ivec3 pos) const {
    auto _ = getReadLock();
    return blockDamages.count(pos) ? blockDamages.at(pos).curr : 0;
}

double DimensionBase::setBlockDamage(glm::ivec3 pos, double damage) {
    if (blockDamages.count(pos)) blockDamages[pos].curr = damage;
    else {
        double health = game->getDefs().blockFromId(getBlock(pos)).health;
        auto _ = getWriteLock();
        blockDamages.insert({pos, Damage { damage, health }});
    }

    return getBlockDamage(pos);
}

unsigned int DimensionBase::getBiome(glm::ivec3 pos) const {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    if (!chunk) return 0;

    return chunk->getBiome(Space::Block::relative::toChunk(pos));
}

bool DimensionBase::setBiome(glm::ivec3 pos, unsigned int biome) {
    auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
    if (!chunk) return false;

    return chunk->setBiome(Space::Block::relative::toChunk(pos), biome);
}

std::shared_ptr<Region> DimensionBase::getOrCreateRegion(glm::ivec3 pos) {
    auto _ = getWriteLock();
    if (regions[pos]) return regions[pos];
    regions[pos] = std::make_shared<Region>(pos);
    return regions[pos];
}

std::shared_ptr<MapBlock> DimensionBase::getOrCreateMapBlock(glm::ivec3 mapBlockPosition) {
    auto region = getOrCreateRegion(Space::Region::world::fromMapBlock(mapBlockPosition));
    unsigned int index = Space::MapBlock::index(mapBlockPosition);

    if (region->get(index) != nullptr) return region->get(index);
    region->set(index, std::make_shared<MapBlock>(mapBlockPosition));
    return region->get(index);
}

SubgamePtr DimensionBase::getGame() {
    return game;
}

World& DimensionBase::getWorld() {
    return world;
}

std::shared_ptr<Chunk> DimensionBase::combinePartials(std::shared_ptr<Chunk> a, std::shared_ptr<Chunk> b) {
    std::shared_ptr<Chunk> src;
    std::shared_ptr<Chunk> res;

    if (a->isGenerated()) {
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

    res->setGenerated(src->isGenerated() || res->isGenerated());
    res->setPartial(!res->isGenerated());
    res->countRenderableBlocks();
    return res;
}

std::shared_ptr<MapGen> DimensionBase::getGen() {
    return mapGen;
}
