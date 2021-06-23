#pragma once

#include "BiomeAtlas.h"

class ServerBiomeAtlas : public BiomeAtlas {
public:
	ServerBiomeAtlas(u32 seed);
	
	void registerBiome(BiomeDef* def) override;
	
	u32 getSeed();
	
private:
	u32 seed;
};
