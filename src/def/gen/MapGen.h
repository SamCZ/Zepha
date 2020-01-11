//
// Created by aurailus on 28/01/19.
//

#pragma once


#include <glm/vec3.hpp>
#include <vector>
#include <noise/noise.h>

#include "MapGenJob.h"
#include "../LocalDefs.h"
#include "BiomeAtlas.h"
#include "../../world/chunk/BlockChunk.h"

using namespace noise;

class MapGen {
public:
	MapGen(unsigned int seed, DefinitionAtlas& atlas, BiomeAtlas& biome);
	std::vector<BlockChunk*> generateMapBlock(glm::vec3 mbPos);
private:
	void generateChunk(std::array<std::pair<MapGenJob*, BlockChunk*>, 64>& chunks, glm::vec3 localPos, glm::vec3 worldPos);

	void buildDensityMap(MapGenJob* job, const glm::vec3& worldPos);
	void buildElevationMap(std::array<std::pair<MapGenJob*, BlockChunk*>, 64>& chunks,
		std::pair<MapGenJob*, BlockChunk*>& chunk, const glm::vec3& localPos, const glm::vec3& worldPos);

	void populateChunk(std::pair<MapGenJob*, BlockChunk*>& chunk, const glm::vec3& worldPos);

	unsigned int seed = 0;

	DefinitionAtlas& atlas;
	BiomeAtlas& biomes;

	module::Perlin temperatureBase;
	module::ScaleBias temperature;

	module::Perlin humidityBase;
	module::ScaleBias humidity;

	module::Perlin roughnessBase;
	module::ScaleBias roughness;

//	module::Perlin worldElevationBase;
//	module::ScaleBias worldElevationScaled;
//
//	module::Perlin worldFeatureBase;
//	module::ScaleBias worldFeatureScaled;
//
//	module::Add worldSmoothElevation;
};