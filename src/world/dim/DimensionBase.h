//
// The base superclass for both dimensions. Handles chunk storage and manipulation.
// Does not include lighting mechanics, which are contained in subclass Dimension.
// Created by aurailus on 2019-11-20.
//

#pragma once

#include <unordered_map>

#include "util/Vec.h"
#include "util/Lockable.h"
#include "util/CovariantPtr.h"

class World;
class Chunk;
class MapGen;
class Region;
class Subgame;
class MapBlock;
class DefinitionAtlas;

class DimensionBase : protected Lockable {
public:
	DimensionBase(SubgamePtr game, World& world, const string& identifier, u16 ind, sptr<MapGen> mapGen);
	
	string getIdentifier() const;
	
	u16 getInd();
	
	virtual void update(f64 delta) = 0;
	
	sptr<Region> getRegion(ivec3 regionPosition) const;
	
	void removeRegion(ivec3 pos);
	
	sptr<MapBlock> getMapBlock(ivec3 mapBlockPosition) const;
	
	virtual void removeMapBlock(ivec3 pos);
	
	bool mapBlockGenerated(ivec3 mapBlockPosition);
	
	sptr<Chunk> getChunk(ivec3 chunkPosition) const;
	
	virtual void setChunk(sptr<Chunk> chunk);
	
	virtual void removeChunk(ivec3 pos);
	
	u16 getBlock(ivec3 pos) const;
	
	virtual bool setBlock(ivec3 pos, u16 block);
	
	virtual f64 getBlockDamage(ivec3 pos) const;
	
	virtual f64 setBlockDamage(ivec3 pos, f64 damage);
	
	u16 getBiome(ivec3 pos) const;
	
	virtual bool setBiome(ivec3 pos, u16 biome);
	
	sptr<MapGen> getGen();
	
	SubgamePtr getGame();
	
	World& getWorld();
	
	protected:
	// Combine two chunk partials, or a chunk and a chunk partial.
	// If both are partials `b` takes preference, if one is a fully generated chunk the partial takes preference.
	static sptr<Chunk> combineChunks(sptr<Chunk> a, sptr<Chunk> b);
	
	sptr<MapGen> mapGen;
	SubgamePtr game;
	World& world;
	
	typedef std::unordered_map<ivec3, sptr<Region>, Vec::ivec3> block_region_map;
	block_region_map regions;
	
	u16 ind;
	string identifier;
	
	struct Damage {
		f64 curr;
		f64 max;
	};
	std::unordered_map<ivec3, Damage, Vec::ivec3> blockDamages;
	
	private:
	inline sptr<Region> getOrCreateRegion(ivec3 pos);
	
	inline sptr<MapBlock> getOrCreateMapBlock(ivec3 mapBlockPosition);
};
