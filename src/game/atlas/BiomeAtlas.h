#pragma once

#include <unordered_map>

#include "util/Types.h"

class BiomeDef;

/**
 * Stores biome definitions, allowing access by ID or identifier.
 */

class BiomeAtlas {
public:
	BiomeAtlas() = default;
	
	virtual void registerBiome(BiomeDef* def) = 0;
	
	BiomeDef& biomeFromId(u16 index);
	
	BiomeDef& biomeFromStr(const string& identifier);
	
	vec<BiomeDef*> biomesFromTag(const string& tag);
	
	u16 size();
	
	const static u16 INVALID = 0;
protected:
	vec<BiomeDef*> defs;
	std::unordered_map<string, u32> defTable;
};