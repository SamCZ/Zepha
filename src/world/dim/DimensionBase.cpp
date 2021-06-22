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

DimensionBase::DimensionBase(SubgamePtr game, World& world, const string& identifier, u16 ind, sptr<MapGen> mapGen) :
	game(game), world(world), identifier(identifier), ind(ind), mapGen(std::move(mapGen)) {}

string DimensionBase::getIdentifier() const {
	return identifier;
}

u16 DimensionBase::getInd() {
	return ind;
}

sptr<Region> DimensionBase::getRegion(ivec3 regionPosition) const {
	auto _ = getReadLock();
	if (!regions.count(regionPosition)) return nullptr;
	return regions.at(regionPosition);
}

void DimensionBase::removeRegion(ivec3 pos) {
	auto _ = getWriteLock();
	regions.erase(pos);
}

sptr<MapBlock> DimensionBase::getMapBlock(ivec3 mapBlockPosition) const {
	auto region = getRegion(Space::Region::world::fromMapBlock(mapBlockPosition));
	if (!region) return nullptr;
	return region->get(Space::MapBlock::index(mapBlockPosition));
}

void DimensionBase::removeMapBlock(ivec3 pos) {
	auto region = getRegion(Space::Region::world::fromMapBlock(pos));
	if (!region) return;
	auto i = Space::MapBlock::index(pos);
	region->remove(i);
	if (region->count == 0) removeRegion(Space::Region::world::fromMapBlock(pos));
}

bool DimensionBase::mapBlockGenerated(ivec3 mapBlockPosition) {
	auto mb = getMapBlock(mapBlockPosition);
	return mb && mb->generated;
}

sptr<Chunk> DimensionBase::getChunk(ivec3 chunkPosition) const {
	auto mapBlock = getMapBlock(Space::MapBlock::world::fromChunk(chunkPosition));
	if (!mapBlock) return nullptr;
	return mapBlock->get(Space::Chunk::index(chunkPosition));
}

void DimensionBase::setChunk(sptr<Chunk> chunk) {
	auto mapBlock = getOrCreateMapBlock(Space::MapBlock::world::fromChunk(chunk->getPos()));
	mapBlock->set(Space::Chunk::index(chunk->getPos()), chunk);
}

void DimensionBase::removeChunk(ivec3 pos) {
	auto mapBlock = getMapBlock(Space::MapBlock::world::fromChunk(pos));
	if (!mapBlock) return;
	auto i = Space::Chunk::index(pos);
	mapBlock->remove(i);
	if (mapBlock->count == 0) removeMapBlock(Space::MapBlock::world::fromChunk(pos));
}

u16 DimensionBase::getBlock(ivec3 pos) const {
	auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
	if (!chunk) return 0;
	
	return chunk->getBlock(Space::Block::relative::toChunk(pos));
}

bool DimensionBase::setBlock(ivec3 pos, u16 block) {
	auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
	if (!chunk) return false;
	
	bool manip = chunk->setBlock(Space::Block::relative::toChunk(pos), block);
	setBlockDamage(pos, 0);
	return manip;
}


f64 DimensionBase::getBlockDamage(ivec3 pos) const {
	auto _ = getReadLock();
	return blockDamages.count(pos) ? blockDamages.at(pos).curr : 0;
}

f64 DimensionBase::setBlockDamage(ivec3 pos, f64 damage) {
	if (blockDamages.count(pos)) blockDamages[pos].curr = damage;
	else {
		double health = game->getDefs().blockFromId(getBlock(pos)).health;
		auto _ = getWriteLock();
		blockDamages.insert({ pos, Damage{ damage, health }});
	}
	
	return getBlockDamage(pos);
}

u16 DimensionBase::getBiome(ivec3 pos) const {
	auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
	if (!chunk) return 0;
	
	return chunk->getBiome(Space::Block::relative::toChunk(pos));
}

bool DimensionBase::setBiome(ivec3 pos, u16 biome) {
	auto chunk = getChunk(Space::Chunk::world::fromBlock(pos));
	if (!chunk) return false;
	
	return chunk->setBiome(Space::Block::relative::toChunk(pos), biome);
}

sptr<Region> DimensionBase::getOrCreateRegion(ivec3 pos) {
	auto _ = getWriteLock();
	if (regions[pos]) return regions[pos];
	regions[pos] = make_shared<Region>(pos);
	return regions[pos];
}

sptr<MapBlock> DimensionBase::getOrCreateMapBlock(ivec3 mapBlockPosition) {
	auto region = getOrCreateRegion(Space::Region::world::fromMapBlock(mapBlockPosition));
	unsigned int index = Space::MapBlock::index(mapBlockPosition);
	
	if (region->get(index) != nullptr) return region->get(index);
	region->set(index, make_shared<MapBlock>(mapBlockPosition));
	return region->get(index);
}

SubgamePtr DimensionBase::getGame() {
	return game;
}

World& DimensionBase::getWorld() {
	return world;
}

sptr<Chunk> DimensionBase::combineChunks(sptr<Chunk> a, sptr<Chunk> b) {
	if (a->isGenerated()) return (a->combineWith(b), a);
	else return (b->combineWith(a), b);
}

sptr<MapGen> DimensionBase::getGen() {
	return mapGen;
}
