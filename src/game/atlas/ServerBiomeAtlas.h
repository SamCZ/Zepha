//
// Created by aurailus on 2019-11-18.
//

#pragma once

#include "BiomeAtlas.h"

class ServerBiomeAtlas : public BiomeAtlas {
	public:
	ServerBiomeAtlas(unsigned int seed);
	
	void registerBiome(BiomeDef* def) override;
	
	unsigned int seed;
};
