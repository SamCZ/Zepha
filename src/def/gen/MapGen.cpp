#include <cmath>

//
// Created by aurailus on 28/01/19.
//

#include "MapGen.h"
#include "NoiseSample.h"
#include "../../game/scene/world/Schematic.h"

MapGen::MapGen(unsigned int seed, DefinitionAtlas& defs, BiomeAtlas& biomes) :
    seed(seed),
    defs(defs),
    biomes(biomes) {

	temperatureBase.SetSeed(seed);
	temperatureBase.SetFrequency(0.02);
	temperatureBase.SetOctaveCount(4);
	temperatureTurbulence.SetSourceModule(0, temperatureBase);
	temperatureTurbulence.SetRoughness(4);
	temperatureTurbulence.SetFrequency(0.2);
	temperature.SetSourceModule(0, temperatureTurbulence);
	temperature.SetScale(0.35);
	temperature.SetBias(0.25);

	humidityBase.SetSeed(seed + 5);
	humidityBase.SetFrequency(0.02);
	humidityBase.SetOctaveCount(4);
    humidityTurbulence.SetSourceModule(0, humidityBase);
    humidityTurbulence.SetRoughness(4);
    humidityTurbulence.SetFrequency(0.2);
	humidity.SetSourceModule(0, humidityTurbulence);
	humidity.SetScale(0.5);
	humidity.SetBias(0.5);

	roughnessBase.SetSeed(seed + 10);
	roughnessBase.SetFrequency(0.02);
	roughnessBase.SetOctaveCount(4);
    roughnessTurbulence.SetSourceModule(0, roughnessBase);
    roughnessTurbulence.SetRoughness(4);
    roughnessTurbulence.SetFrequency(0.2);
	roughness.SetSourceModule(0, roughnessTurbulence);
	roughness.SetScale(0.5);
	roughness.SetBias(0.5);

	treeMap.SetFrequency(1.25);
	treeMap.SetOctaveCount(4);
	treeAbs.SetSourceModule(0, treeMap);
}

MapGen::chunk_partials_map MapGen::generateMapBlock(glm::ivec3 mbPos) {
	chunk_partials_map chunks {};

	for (short i = 3; i >= 0; i--) {
		for (short j = 0; j < 4; j++) {
			for (short k = 0; k < 4; k++) {
				glm::ivec3 pos = glm::ivec3(j, i, k) + (mbPos * 4);
				generateChunk(chunks, pos);
			}
		}
	}

	for (auto& chunk : chunks) {
	    // Delete MapGenJobs
	    delete chunk.second.first;
	    chunk.second.first = nullptr;
	}

	return chunks;
}

void MapGen::generateChunk(chunk_partials_map& chunks, glm::ivec3 worldPos) {
    if (chunks.count(worldPos) == 0) chunks.insert(std::pair<glm::ivec3, chunk_partial>{worldPos, {new MapGenJob(), new BlockChunk()}});
    auto& chunk = chunks.at(worldPos);
	chunk.second->pos = worldPos;

	buildDensityMap(chunk.first, worldPos);
	buildElevationMap(chunks, chunk);

    generateBlocks(chunk);
    generateStructures(chunks, chunk);

    chunk.second->calcNonAirBlocks();
    chunk.second->generated = true;
}

void MapGen::buildDensityMap(MapGenJob* job, glm::ivec3 worldPos) {
    job->temperature = NoiseSample(temperature, {worldPos.x, 0, worldPos.z}, {4, 1}, true);
    job->humidity    = NoiseSample(humidity,    {worldPos.x, 0, worldPos.z}, {4, 1}, true);
    job->roughness   = NoiseSample(roughness,   {worldPos.x, 0, worldPos.z}, {4, 1}, true);

    // TODO: This is... a travesty. Please stop doing this weird jank insertion into a
    // noisesample and create a proper constructor or *something*... could probs use a module owo

    auto biome = biomes.getBiomeAt(job->temperature.get({}), job->humidity.get({}), job->roughness.get({}));
    auto terrain = NoiseSample({4, 4});

    float offsetH = 16.f / 4.f;
    float offsetV = 16.f / 4.f;

    for (int i = 0; i <= 4; i++) {
        for (int j = 0; j <= 4; j++) {
            for (int k = 0; k <= 4; k++) {
                glm::vec3 localPos = {(offsetH * i) / 1.01f, (offsetV * j) / 1.01f, (offsetH * k) / 1.01f};
                glm::vec3 pos = {(worldPos.x * 16 + offsetH * i) / 16.f, (worldPos.y * 16 + offsetV * j) / 16.f, (worldPos.z * 16 + offsetH * k) / 16.f};
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

void MapGen::buildElevationMap(chunk_partials_map& chunks, chunk_partial& chunk) {
    glm::ivec3 worldPos = chunk.second->pos;

    MapGenJob* upperJob = nullptr;
    bool createdUpperJob = false;

	for (int i = 0; i < 256; i++) {
		const int x = i % 16;
		const int z = i / 16;

		short depth = 16;

		if (chunk.first->density[Space::Block::index({x, 15, z})] > 0) {
		    if (!upperJob) {
                glm::ivec3 rel = worldPos + glm::ivec3 {0, 1, 0};
                if (chunks.count(rel) != 0) upperJob = chunks.at(rel).first;
                else {
                    upperJob = new MapGenJob();
                    buildDensityMap(upperJob, rel);
                    createdUpperJob = true;
                }
            }

			for (int j = 0; j < 16; j++) {
				if (upperJob->density[Space::Block::index({ x, j, z })] <= 0) {
					depth = j;
					break;
				}
			}
		}
		else depth = 0;

		for (int y = 15; y >= 0; y--) {
			int ind = Space::Block::index({ x, y, z });
			depth = (chunk.first->density[ind] > 0 ? std::min(depth + 1, 16) : 0);
			chunk.first->depth[ind] = depth + (chunk.first->density[ind] - static_cast<int>(chunk.first->density[ind]));
		}
	}

	if (createdUpperJob) delete upperJob;
}

void MapGen::generateBlocks(chunk_partial& chunk) {
	glm::ivec3 lp;

	for (unsigned short m = 0; m < 4096; m++) {
	    if (chunk.second->getBlock(m) != DefinitionAtlas::INVALID) continue;
		Vec::indAssignVec(m, lp);

		auto biome = biomes.getBiomeAt(chunk.first->temperature.get(lp), chunk.first->humidity.get(lp), chunk.first->roughness.get(lp));
		chunk.second->biomes[m] = biome.index;

		int d = std::floor(chunk.first->depth[m]);

		chunk.second->blocks[m]
			= d <= 1 ? DefinitionAtlas::AIR
			: d <= 2 ? biome.topBlock
			: d <= 4 ? biome.soilBlock
			: biome.rockBlock;
	}
}

void MapGen::generateStructures(chunk_partials_map& chunks, chunk_partial& chunk) {
    unsigned int cWood = defs.blockFromStr("zeus:default:wood").index;
    unsigned int cLeaves = defs.blockFromStr("zeus:default:leaves").index;
    unsigned int cAir = DefinitionAtlas::INVALID;

    Schematic c {};
    c.dimensions = {3, 3, 3};
    c.origin = {1, 0, 1};
    c.blocks = { cAir, cAir, cAir, cAir, cLeaves, cAir, cAir, cAir, cAir,
                 cAir, cWood, cAir, cLeaves, cWood, cLeaves, cAir, cLeaves, cAir,
                 cAir, cAir, cAir, cAir, cLeaves, cAir, cAir, cAir, cAir };

    glm::ivec3 wp = chunk.second->pos;
    glm::ivec3 lp;

    for (unsigned short m = 0; m < 4096; m++) {
        Vec::indAssignVec(m, lp);

        glm::ivec3 p = wp * 16 + lp;

        if (treeAbs.GetValue(p.x, p.y, p.z) > 1.2 && chunk.first->depth[m] <= 2 && chunk.first->depth[m] > 1) {
            glm::ivec3 off = {};
            for (unsigned int i = 0; i < c.length(); i++) {
                c.assignOffset(i, off);
                setBlock(p + off - c.origin, c.blocks[i], chunks);
            }
        }
    }
}

void MapGen::setBlock(glm::ivec3 worldPos, unsigned int block, MapGen::chunk_partials_map &chunks) {
    if (block == DefinitionAtlas::INVALID) return;

    glm::ivec3 chunkPos = Space::Chunk::world::fromBlock(worldPos);
    BlockChunk* chunk = nullptr;

    if (chunks.count(chunkPos)) chunk = chunks.at(chunkPos).second;
    else {
        chunk = new BlockChunk();
        chunk->pos = chunkPos;
        chunks.insert(std::pair<glm::ivec3, chunk_partial>{chunkPos, {new MapGenJob(), chunk}});
    }

    unsigned int index = Space::Block::index(worldPos);
    if (chunk->getBlock(index) <= DefinitionAtlas::AIR) chunk->setBlock(index, block);
}

std::shared_ptr<BlockChunk> MapGen::combinePartials(std::shared_ptr<BlockChunk> a, std::shared_ptr<BlockChunk> b) {
    std::shared_ptr<BlockChunk> src;
    std::shared_ptr<BlockChunk> res;

    if (a->generated) {
        res = a;
        src = b;
    }
    else {
        res = b;
        src = a;
    }

    for (unsigned int i = 0; i < 4096; i++) {
        if (src->getBlock(i) != DefinitionAtlas::INVALID) res->setBlock(i, src->getBlock(i));
    }

    res->generated = src->generated || res->generated;
    return res;
}
