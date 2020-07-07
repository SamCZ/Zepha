//
// Created by aurailus on 28/01/19.
//

#include <random>
#include <cmath>

#include "MapGen.h"

#include "BiomeDef.h"
#include "MapGenJob.h"
#include "BiomeAtlas.h"
#include "MapGenProps.h"
#include "NoiseSample.h"
#include "../item/BlockDef.h"
#include "../DefinitionAtlas.h"
#include "../../world/chunk/Chunk.h"
#include "../../game/scene/world/Schematic.h"

MapGen::MapGen(unsigned int seed, DefinitionAtlas& defs, BiomeAtlas& biomes, std::shared_ptr<MapGenProps> props) :
    seed(seed),
    defs(defs),
    props(props),
    biomes(biomes) {}

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

    generateSunlight(chunks, mbPos);

	for (auto& chunk : chunks) {
	    // Delete MapGenJobs
	    delete chunk.second.first;
	    chunk.second.first = nullptr;
	}

	return chunks;
}

void MapGen::generateChunk(chunk_partials_map& chunks, glm::ivec3 worldPos) {
    if (chunks.count(worldPos) == 0) chunks.insert(std::pair<glm::ivec3, chunk_partial>{worldPos, {new MapGenJob(), new Chunk()}});
    auto& chunk = chunks.at(worldPos);
	chunk.second->pos = worldPos;

	buildDensityMap(chunk.first, worldPos);
	buildElevationMap(chunks, chunk);

    generateBlocks(chunk);
    generateStructures(chunks, chunk);

    chunk.second->recalculateRenderableBlocks();
    chunk.second->generated = true;
}

void MapGen::buildDensityMap(MapGenJob* job, glm::ivec3 worldPos) {
    job->temperature = {}; job->humidity = {}; job->roughness = {};

    job->temperature.fill([&](glm::ivec3 pos) {
        return props->temperature.GetValue(worldPos.x + pos.x / 16.f, 0, worldPos.z + pos.z / 16.f); }, 4);
    job->humidity.fill([&](glm::ivec3 pos) {
        return props->humidity.GetValue(worldPos.x + pos.x / 16.f, 0, worldPos.z + pos.z / 16.f); }, 4);
    job->roughness.fill([&](glm::ivec3 pos) {
        return props->roughness.GetValue(worldPos.x + pos.x / 16.f, 0, worldPos.z + pos.z / 16.f); }, 4);

    NoiseSample volume = {}, heightmap = {};

    volume.fill([&](glm::ivec3 pos) {
        auto& biome = biomes.getBiomeAt(job->temperature.get(pos), job->humidity.get(pos), job->roughness.get(pos));
        return biome.volume[biome.volume.size() - 1]->GetValue(worldPos.x + pos.x / 16.f, worldPos.y + pos.y / 16.f, worldPos.z + pos.z / 16.f);
    }, {4, 4});

    heightmap.fill([&](glm::ivec3 pos) {
        auto& biome = biomes.getBiomeAt(job->temperature.get(pos), job->humidity.get(pos), job->roughness.get(pos));
        return biome.heightmap[biome.heightmap.size() - 1]->GetValue(worldPos.x + pos.x / 16.f, 0, worldPos.z + pos.z / 16.f);
    }, 4);

	glm::ivec3 lp;
	for (int m = 0; m < 4096; m++) {
		Vec::indAssignVec(m, lp);
		job->density[m] = (volume.get(lp) + heightmap.get(lp)) - (lp.y + worldPos.y * 16);
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
    std::unique_ptr<Chunk> dupe = nullptr;
    if (chunk.second->partial) dupe = std::make_unique<Chunk>(*chunk.second);

    chunk.second->blocks = {};
    chunk.second->biomes = {};

    std::array<std::array<unsigned short, 16>, 16> biomeArray {};
    for (unsigned short x = 0; x < 16; x++) {
        biomeArray[x] = {};
        for (unsigned short z = 0; z < 16; z++) {
            glm::ivec3 lp = {x, 0, z};
            biomeArray[x][z] = biomes.getBiomeAt(
                    chunk.first->temperature.get(lp),
                    chunk.first->humidity.get(lp),
                    chunk.first->roughness.get(lp)).index;
        }
    }

    for (unsigned short m = 0; m < 4096; m++) {
        glm::ivec3 lp = Space::Block::fromIndex(m);
        auto& biome = biomes.biomeFromId(biomeArray[lp.x][lp.z]);

        unsigned int storedBlock = (chunk.second->blocks.size() <= 0 ? -1 : chunk.second->blocks[chunk.second->blocks.size() - 1]);
        unsigned int storedBiome = (chunk.second->biomes.size() <= 0 ? -1 : chunk.second->biomes[chunk.second->biomes.size() - 1]);

        if (biome.index != storedBiome) {
            chunk.second->biomes.emplace_back(m);
            chunk.second->biomes.emplace_back(biome.index);
        }

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

    if (dupe) for (unsigned short i = 0; i < 4096; i++) {
        unsigned int b = dupe->getBlock(i);
        if (b != DefinitionAtlas::INVALID) chunk.second->setBlock(i, b);
    }
}

void MapGen::generateStructures(chunk_partials_map& chunks, chunk_partial& chunk) {
    std::default_random_engine generator(chunk.second->pos.x + chunk.second->pos.y * 30 + chunk.second->pos.z * 3.5);
    std::uniform_real_distribution<float> distribution(0, 1);

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

                    auto biome = biomes.biomeFromId(chunk.second->getBiome(ind));
                    auto schematic = biome.schematics.size() > 0 ? biome.schematics[0] : nullptr;

                    if (schematic != nullptr) {
                        if (!schematic->processed) schematic->process(defs);
                        for (unsigned int j = 0; j < schematic->length(); j++) {
                            schematic->assignOffset(j, off);
                            setBlock(p + off - schematic->origin, schematic->blocks[j], chunks);
                        }
                    }
                }
            }
        }
    }
}

void MapGen::generateSunlight(MapGen::chunk_partials_map &chunks, glm::ivec3 mbPos) {
    std::queue<SunlightNode> sunlightQueue;

    glm::ivec3 c {};
    for (c.x = 0; c.x < 4; c.x++) {
        for (c.z = 0; c.z < 4; c.z++) {
            c.y = 3;
            Chunk* chunk = chunks[mbPos * 4 + c].second;

            glm::ivec3 b {};
            for (b.x = 0; b.x < 16; b.x++) {
                for (b.z = 0; b.z < 16; b.z++) {
                    b.y = 15;

                    while (true) {
                        unsigned int ind =  Space::Block::index(b);
                        if (defs.blockFromId(chunk->getBlock(ind)).lightPropagates) {
                            chunk->setLight(ind, 3, 15);
                            sunlightQueue.emplace(ind, chunk);
                        }
                        else {
                            c.y = 3;
                            chunk = chunks[mbPos * 4 + c].second;
                            break;
                        }

                        b.y--;
                        if (b.y < 0) {
                            b.y = 15;
                            c.y = c.y ? c.y - 1 : 3;
                            chunk = chunks[mbPos * 4 + c].second;
                            if (c.y == 3) break;
                        }
                    }
                }
            }
        }
    }

    propogateSunlightNodes(chunks, sunlightQueue);
}

bool MapGen::containsWorldPos(Chunk *chunk, glm::ivec3 pos) {
    return chunk && Space::Chunk::world::fromBlock(pos) == chunk->pos;
}

void MapGen::propogateSunlightNodes(MapGen::chunk_partials_map &chunks, std::queue<SunlightNode> &queue) {
    while (!queue.empty()) {
        SunlightNode& node = queue.front();

        unsigned char lightLevel = node.chunk->getLight(node.index, 3);
        glm::ivec3 worldPos = node.chunk->pos * 16 + Space::Block::fromIndex(node.index);

        for (const auto& i : Vec::adj) {
            glm::ivec3 check = worldPos + i;

            Chunk* chunk;
            if (containsWorldPos(node.chunk, check)) chunk = node.chunk;
            else {
                glm::ivec3 worldPos = Space::Chunk::world::fromBlock(check);
                if (!chunks.count(worldPos)) continue;
                chunk = chunks[worldPos].second;
                if (!chunk) continue;
            }

            auto ind = Space::Block::index(check);
            if (defs.blockFromId(chunk->getBlock(ind)).lightPropagates && chunk->getLight(ind, 3) + 2 <= lightLevel) {
                chunk->setLight(ind, 3, lightLevel - static_cast<int>(!(lightLevel == 15 && i.y == -1)));
                queue.emplace(ind, chunk);
            }
        }

        queue.pop();
    }
}

void MapGen::setBlock(glm::ivec3 worldPos, unsigned int block, MapGen::chunk_partials_map &chunks) {
    if (block == DefinitionAtlas::INVALID) return;

    glm::ivec3 chunkPos = Space::Chunk::world::fromBlock(worldPos);
    Chunk* chunk = nullptr;

    if (chunks.count(chunkPos)) chunk = chunks.at(chunkPos).second;
    else {
        chunk = new Chunk();
        chunk->pos = chunkPos;
        chunk->partial = true;
        chunks.insert(std::pair<glm::ivec3, chunk_partial>{chunkPos, {new MapGenJob(), chunk}});
    }

    unsigned int index = Space::Block::index(worldPos);
    if (chunk->getBlock(index) <= DefinitionAtlas::AIR) chunk->setBlock(index, block);
}

std::shared_ptr<Chunk> MapGen::combinePartials(std::shared_ptr<Chunk> a, std::shared_ptr<Chunk> b) {
    std::shared_ptr<Chunk> src;
    std::shared_ptr<Chunk> res;

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
    res->recalculateRenderableBlocks();
    return res;
}
