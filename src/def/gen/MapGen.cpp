//
// Created by aurailus on 28/01/19.
//

#include "MapGen.h"
#include "NoiseSample.h"
#include "../../game/scene/world/Schematic.h"
#include <random>
#include <cmath>

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
    glm::ivec3 lp {};

    auto dupe = std::make_unique<BlockChunk>(*chunk.second);

    chunk.second->blocks = {};
    chunk.second->biomes = {};

    std::array<std::array<unsigned short, 16>, 16> biomeArray {};
    for (unsigned short x = 0; x < 16; x++) {
        biomeArray[x] = {};
        for (unsigned short z = 0; z < 16; z++) {
            lp = {x, 0, z};
            biomeArray[x][z] = biomes.getBiomeAt(
                    chunk.first->temperature.get(lp),
                    chunk.first->humidity.get(lp),
                    chunk.first->roughness.get(lp)).index;
        }
    }

    for (unsigned short m = 0; m < 4096; m++) {
        Vec::indAssignVec(m, lp);
        auto& biome = biomes.biomeFromId(biomeArray[lp.x][lp.z]);

        unsigned int storedBlock = (chunk.second->blocks.size() <= 0 ? -1 : chunk.second->blocks[chunk.second->blocks.size() - 1]);
        unsigned int storedBiome = (chunk.second->biomes.size() <= 0 ? -1 : chunk.second->biomes[chunk.second->biomes.size() - 1]);

        if (biome.index != storedBiome) {
            chunk.second->biomes.emplace_back(m);
            chunk.second->biomes.emplace_back(biome.index);
        }

//        if (chunk.second->blocks[ind] != DefinitionAtlas::INVALID) continue;

        int d = std::floor(chunk.first->depth[m]);
        unsigned int targetBlock
                = d <= 1 ? DefinitionAtlas::AIR
                : d <= 2 ? biome.topBlock
                : d <= 4 ? biome.soilBlock
                : biome.rockBlock;

        if (targetBlock != storedBlock) {
            chunk.second->blocks.emplace_back(m);
            chunk.second->blocks.emplace_back(targetBlock);
        }
    }

    if (dupe->partial) for (unsigned short i = 0; i < 4096; i++) {
        unsigned int b = dupe->getBlock(i);
        if (b != DefinitionAtlas::INVALID) chunk.second->setBlock(i, b);
    }
}

void MapGen::generateStructures(chunk_partials_map& chunks, chunk_partial& chunk) {
    std::default_random_engine generator(chunk.second->pos.x + chunk.second->pos.y * 30 + chunk.second->pos.z * 3.5);
    std::uniform_real_distribution<float> distribution(0, 1);

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

    for (unsigned short i = 0; i < 256; i++) {
        unsigned short x = i / 16;
        unsigned short z = i % 16;

        if (distribution(generator) > 0.97) {
            for (unsigned short y = 0; y < 16; y++) {
                lp = {x, y, z};
                unsigned short ind = Space::Block::index(lp);

                if (chunk.first->depth[ind] > 0 && chunk.first->depth[ind] <= 1.1) {

                    glm::ivec3 off = {};
                    glm::ivec3 p = wp * 16 + lp;

                    for (unsigned int j = 0; j < c.length(); j++) {
                        c.assignOffset(j, off);
                        setBlock(p + off - c.origin, c.blocks[j], chunks);
                    }
                }
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
        chunk->initializeEmpty();
        chunk->pos = chunkPos;
        chunk->partial = true;
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
        if (src->getBlock(i) > DefinitionAtlas::INVALID) res->setBlock(i, src->getBlock(i));
    }

    res->generated = src->generated || res->generated;
    res->partial = !res->generated;
    return res;
}
