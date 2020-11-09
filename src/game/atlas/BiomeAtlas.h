//
// Manages biome definitions and Voronoi map.
// Created by aurailus on 2019-11-13.
//

#pragma once

#include <vector>
#include <unordered_map>

class BiomeDef;

class BiomeAtlas {
	public:
	BiomeAtlas() = default;
	
	virtual void registerBiome(BiomeDef* def) = 0;
	
	BiomeDef& biomeFromId(unsigned int index);
	
	BiomeDef& biomeFromStr(const std::string& identifier);
	
	std::vector<BiomeDef*> biomesFromTag(const std::string& tag);
	
	unsigned int size();
	
	const static unsigned int INVALID = 0;
	protected:
	std::vector<BiomeDef*> defs;
	std::unordered_map<std::string, unsigned int> defTable;
};