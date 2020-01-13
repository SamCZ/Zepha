#include <cmath>

//
// Created by aurailus on 28/01/19.
//

#include "MapGen.h"
#include "../../util/Timer.h"
#include "NoiseSample.h"

MapGen::MapGen(unsigned int seed, DefinitionAtlas& atlas, BiomeAtlas& biomes) :
	seed(seed),
	atlas(atlas),
	biomes(biomes) {

	temperatureBase.SetSeed(seed);
	temperatureBase.SetFrequency(0.02);
	temperatureBase.SetOctaveCount(4);
//	temperatureTurbulance.Set
	temperature.SetSourceModule(0, temperatureBase);
	temperature.SetScale(0.35);
	temperature.SetBias(0.25);

	humidityBase.SetSeed(seed + 5);
	humidityBase.SetFrequency(0.02);
	humidityBase.SetOctaveCount(4);
	humidity.SetSourceModule(0, humidityBase);
	humidity.SetScale(0.5);
	humidity.SetBias(0.5);

	roughnessBase.SetSeed(seed + 10);
	roughnessBase.SetFrequency(0.02);
	roughnessBase.SetOctaveCount(4);
	roughness.SetSourceModule(0, roughnessBase);
	roughness.SetScale(0.5);
	roughness.SetBias(0.5);
}

std::vector<BlockChunk*> MapGen::generateMapBlock(glm::vec3 mbPos) {
	std::array<std::pair<MapGenJob*, BlockChunk*>, 64> chunks{};

	// Go top down
	for (short i = 3; i >= 0; i--) {
		for (short j = 0; j < 4; j++) {
			for (short k = 0; k < 4; k++) {
				glm::vec3 pos{ j, i, k };
				generateChunk(chunks, pos, pos + mbPos * 4.f);
			}
		}
	}

	std::vector<BlockChunk*> returns{};
	returns.reserve(64);
	for (auto& pair : chunks) {
		returns.push_back(pair.second);
	}

	return returns;
}

void MapGen::generateChunk(std::array<std::pair<MapGenJob*, BlockChunk*>, 64>& chunks, glm::vec3 localPos, glm::vec3 worldPos) {
	unsigned short index = localPos.x + 4 * (localPos.y + 4 * localPos.z);
	auto& chunk = chunks[index];
	chunk.first = new MapGenJob();
	chunk.second = new BlockChunk();
	chunk.second->pos = worldPos;

	buildDensityMap(chunk.first, worldPos);
	buildElevationMap(chunks, chunk, localPos, worldPos);

	populateChunk(chunk, worldPos);
}

void MapGen::buildDensityMap(MapGenJob* job, const glm::vec3& worldPos) {
    job->temperature = NoiseSample(temperature, worldPos, {4, 4}, false);
    job->humidity    = NoiseSample(humidity,    worldPos, {4, 4}, false);
    job->roughness   = NoiseSample(roughness,   worldPos, {4, 4}, false);

    auto biome = biomes.getBiomeAt(job->temperature.get({}), job->humidity.get({}), job->roughness.get({}));
//    auto terrain = NoiseSample(*biome.modules[biome.modules.size() - 1], worldPos, {4, 1}, true);

    auto terrain = NoiseSample({4, 1});

    float offsetH = 16.f / 4.f;
    float offsetV = 16.f / 1.f;

    for (int i = 0; i <= 4; i++) {
        for (int j = 0; j <= 1; j++) {
            for (int k = 0; k <= 4; k++) {
                //TODO: Find out why this is being stoopd
                glm::vec3 localPos = {(offsetH * i) / 1.01f, 0, (offsetH * k) / 1.01f};
                glm::vec3 pos = {(worldPos.x * 16 + offsetH * i) / 16.f, 0, (worldPos.z * 16 + offsetH * k) / 16.f};
                auto& biome = biomes.getBiomeAt(job->temperature.get(localPos), job->humidity.get(localPos), job->roughness.get(localPos));
                auto& mod = biome.modules[biome.modules.size() - 1];
                terrain.set({i, j, k}, static_cast<float>(mod->GetValue(pos.x, pos.y, pos.z)));
            }
        }
    }

	glm::ivec3 lp;
	for (int m = 0; m < 4096; m++) {
		Vec::indAssignVec(m, lp);
		job->density[m] = terrain.get(lp) - (lp.y + worldPos.y * 16);
	}
}

void MapGen::buildElevationMap(std::array<std::pair<MapGenJob*, BlockChunk*>, 64>& chunks,
	std::pair<MapGenJob*, BlockChunk*>& chunk, const glm::vec3& localPos, const glm::vec3& worldPos) {

	MapGenJob* upperChunk = nullptr;

	for (int i = 0; i < 256; i++) {
		const int x = i % 16;
		const int z = i / 16;

		short depth = 16;

		if (chunk.first->density[Space::Block::index({ x, 15, z })] > 0) {
			if (localPos.y < 3) {
				unsigned short index = localPos.x + 4 * (localPos.y + 1 + 4 * localPos.z);
				upperChunk = chunks[index].first;
			}
			if (upperChunk == nullptr) {
				upperChunk = new MapGenJob();
				buildDensityMap(upperChunk, worldPos + glm::vec3{ 0, 1, 0 });
			}

			for (int j = 0; j < 16; j++) {
				int ind = Space::Block::index({ x, j, z });

				if (upperChunk->density[ind] <= 0) {
					depth = j;
					break;
				}
			}
		}
		else depth = 0;

		for (int y = 15; y >= 0; y--) {
			int ind = Space::Block::index({ x, y, z });

			if (chunk.first->density[ind] > 0) {
				depth = std::min(depth + 1, 16);
			}
			else depth = 0;

			chunk.first->depth[ind] = depth + (chunk.first->density[ind] - static_cast<int>(chunk.first->density[ind]));
		}
	}

	if (localPos.y >= 3) delete upperChunk;
}

void MapGen::populateChunk(std::pair<MapGenJob*, BlockChunk*>& chunk, const glm::vec3& worldPos) {
	glm::ivec3 lp;

	for (int m = 0; m < 4096; m++) {
		Vec::indAssignVec(m, lp);

		auto biome = biomes.getBiomeAt(chunk.first->temperature.get(lp), chunk.first->humidity.get(lp), chunk.first->roughness.get(lp));
		chunk.second->biomes[m] = biome.index;

		int d = std::floor(chunk.first->depth[m]);

		chunk.second->blocks[m] =
			d <= 1 ? DefinitionAtlas::AIR
			: d <= 2 ? biome.topBlock
			: d <= 4 ? biome.soilBlock
			: biome.rockBlock;
	}

	chunk.second->mgRegenEmpty();
}