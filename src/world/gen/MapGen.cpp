//
// Created by aurailus on 28/01/19.
//

#include <random>

#include "MapGen.h"

#include "world/World.h"
#include "game/Subgame.h"
#include "util/Schematic.h"
#include "game/def/BiomeDef.h"
#include "game/def/BlockDef.h"
#include "world/dim/Dimension.h"
#include "game/atlas/BiomeAtlas.h"
#include "world/dim/chunk/Chunk.h"
#include "game/atlas/DefinitionAtlas.h"

MapGen::MapGen(Subgame& game, World& world, unsigned int seed, std::unordered_set<std::string> biomes) :
	game(game), world(world), props(seed) {
	
	std::unordered_set<unsigned int> biomeIndices {};
	for (const auto& str : biomes) {
		if (str[0] == '#')
			for (auto& biome : game.getBiomes().biomesFromTag(str.substr(1, str.length() - 1)))
				biomeIndices.insert(biome->index);
		else biomeIndices.insert(game.getBiomes().biomeFromStr(str).index);
	}
	
	generateVoronoi(biomeIndices);
}

[[maybe_unused]] std::unique_ptr<MapGen::CreatedSet> MapGen::generateChunk(unsigned int dim, glm::ivec3 pos) {
	return generateArea(dim, pos, 1);
}

std::unique_ptr<MapGen::CreatedSet> MapGen::generateMapBlock(unsigned int dim, glm::ivec3 pos) {
	return generateArea(dim, Space::Chunk::world::fromMapBlock(glm::vec3(pos)), 4);
}

std::unique_ptr<MapGen::CreatedSet> MapGen::generateArea(unsigned int dim, glm::ivec3 origin, unsigned int size) {
	Job job(origin, size);
	
	// Build Biome Prop Maps
	
	const auto fill = [&](const noise::module::Module& s) {
		return [&](glm::vec3 pos) {
			glm::vec3 worldPos = glm::vec3(job.pos) + pos * static_cast<float>(job.size);
			return s.GetValue(worldPos.x, 0, worldPos.z);
		};
	};
	
	job.temperature.populate(fill(props.temperature));
	job.roughness.populate(fill(props.roughness));
	job.humidity.populate(fill(props.humidity));
	
	// Generate Biome Topmap
	
	std::vector<unsigned int> biomeMap {};
	biomeMap.resize((job.size * 16 + 1) * (job.size * 16 + 1));
	
	for (unsigned short i = 0; i < biomeMap.size(); i++) {
		glm::vec3 indPos = { i / (job.size * 16 + 1), 0, i % (job.size * 16 + 1) };
		glm::vec3 queryPos = indPos / 16.f / static_cast<float>(job.size);
		
		biomeMap[i] = getBiomeAt(job.temperature.get(queryPos),
			job.humidity.get(queryPos), job.roughness.get(queryPos));
	}
	
	// Generate Heightmap and Volume
	
	job.heightmap.populate([&](glm::vec3 pos) {
		glm::ivec3 blockPos = glm::ivec3(pos * 16.f * static_cast<float>(job.size));
		auto& biome = game.getBiomes().biomeFromId(biomeMap.at(blockPos.x * (job.size * 16 + 1) + blockPos.z));
		glm::vec3 worldPos = glm::vec3(job.pos) + pos * static_cast<float>(job.size);
		return biome.heightmap[biome.heightmap.size() - 1]->GetValue(worldPos.x, 0, worldPos.z);
	});
	
	job.volume.populate([&](glm::vec3 pos) {
		glm::ivec3 blockPos = glm::ivec3(pos * 16.f * static_cast<float>(job.size));
		auto& biome = game.getBiomes().biomeFromId(biomeMap.at(blockPos.x * (job.size * 16 + 1) + blockPos.z));
		glm::vec3 worldPos = glm::vec3(job.pos) + pos * static_cast<float>(job.size);
		return biome.volume[biome.volume.size() - 1]->GetValue(worldPos.x, worldPos.y, worldPos.z);
	});
	
	// Generate Chunks
	
	glm::ivec3 pos {};
	for (pos.x = 0; pos.x < job.size; pos.x++)
		for (pos.z = 0; pos.z < job.size; pos.z++) {
			std::unique_ptr<ChunkData> densityAbove = nullptr;
			for (pos.y = job.size; pos.y >= 0; pos.y--) {
				if (pos.y == job.size) {
					densityAbove = populateChunkDensity(job, pos);
					continue;
				}
				
				std::unique_ptr<ChunkData> density = populateChunkDensity(job, pos);
				std::unique_ptr<ChunkData> depth = populateChunkDepth(density, std::move(densityAbove));
				
				generateChunkBlocks(job, pos, biomeMap, *depth);
				generateChunkStructures(job, pos, biomeMap, *depth);
				
				densityAbove = std::move(density);
			}
		}

//    generateSunlight(chunks, mbPos);
	
	auto created = std::make_unique<CreatedSet>();
	for (const auto& chunk : *job.chunks) {
		created->emplace(chunk.first);
		world.getDimension(dim)->setChunk(chunk.second);
	}
	
	return std::move(created);
}

void MapGen::generateVoronoi(const std::unordered_set<unsigned int>& biomes) {
	std::vector<std::pair<glm::vec3, unsigned short>> points {};
	for (auto biomeInd : biomes) {
		auto& biome = game.getBiomes().biomeFromId(biomeInd);
		
		points.emplace_back(glm::vec3 {
			static_cast<unsigned short>(std::fmin(voronoiSize - 1,
				std::fmax(0, (biome.temperature + 1) / 2 * voronoiSize))),
			static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, biome.humidity * voronoiSize))),
			static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, biome.roughness * voronoiSize)))
		}, biomeInd);
	}
	
	voronoi.setPoints(points);
}

unsigned int MapGen::getBiomeAt(float temperature, float humidity, float roughness) {
	return voronoi.getPoint(
		static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, (temperature + 1) / 2 * voronoiSize))),
		static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, humidity * voronoiSize))),
		static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, roughness * voronoiSize))));
}

std::unique_ptr<MapGen::ChunkData> MapGen::populateChunkDensity(MapGen::Job& job, glm::ivec3 localPos) {
	auto data = std::make_unique<ChunkData>();
	
	for (int i = 0; i < 4096; i++) {
		glm::ivec3 indPos = Space::Block::fromIndex(i);
		glm::vec3 queryPos = (glm::vec3(localPos) + glm::vec3(indPos) / 16.f) / static_cast<float>(job.size);
		(*data)[i] = (job.volume.get(queryPos) + job.heightmap.get({ queryPos.x, 0, queryPos.z })) -
		             ((job.pos.y + localPos.y) * 16 + indPos.y);
	}
	
	return data;
}

std::unique_ptr<MapGen::ChunkData> MapGen::populateChunkDepth(std::unique_ptr<ChunkData>& chunkDensity,
	std::unique_ptr<ChunkData> chunkDensityAbove) {
	
	auto data = std::make_unique<ChunkData>();
	
	for (unsigned short i = 0; i < 256; i++) {
		glm::ivec2 pos = { i / 16, i % 16 };
		short depth = 16;
		
		if ((*chunkDensity)[Space::Block::index({ pos.x, 15, pos.y })] > 0) {
			for (unsigned char j = 0; j < 16; j++) {
				if ((*chunkDensityAbove)[Space::Block::index({ pos.x, j, pos.y })] <= 0) {
					depth = j;
					break;
				}
			}
		}
		else {
			depth = 0;
		}
		
		for (char y = 15; y >= 0; y--) {
			unsigned int ind = Space::Block::index({ pos.x, y, pos.y });
			depth = ((*chunkDensity)[ind] > 0 ? std::min(depth + 1, 16) : 0);
			(*data)[ind] = depth + ((*chunkDensity)[ind] - static_cast<int>((*chunkDensity)[ind]));
		}
	}
	
	return data;
}

void
MapGen::generateChunkBlocks(Job& job, glm::ivec3 localPos, std::vector<unsigned int> biomeMap, ChunkData& depthMap) {
	glm::ivec3 chunkPos = job.pos + localPos;
	
	auto partial = (job.chunks->count(chunkPos) ? job.chunks->at(chunkPos) : nullptr);
	if (partial) job.chunks->erase(chunkPos);
	
	auto& chunk = *(*job.chunks->emplace(chunkPos, std::make_shared<Chunk>(chunkPos)).first).second;
	
	unsigned int cBlockID = 0;
	unsigned int cBiomeID = 0;
	
	unsigned int partialBlock = DefinitionAtlas::INVALID;
	int partialInd = -1;
	unsigned int partialNextAt = 0;
	
	for (unsigned short i = 0; i < 4096; i++) {
		glm::ivec3 indPos = Space::Block::fromIndex(i);
		
		unsigned int biomeID = biomeMap[(localPos.x * 16 + indPos.x) * (job.size * 16 + 1) +
		                                (localPos.z * 16 + indPos.z)];
		auto& biome = game.getBiomes().biomeFromId(biomeID);
		
		if (partial && i >= partialNextAt) {
			partialInd++;
			partialBlock = partial->blocks[partialInd * 2 + 1];
			partialNextAt = (partialInd * 2 + 2 >= partial->blocks.size()) ? 4096 : partial->blocks[partialInd * 2 + 2];
		}
		
		float depth = depthMap[i];
		unsigned int blockID =
			partialBlock != DefinitionAtlas::INVALID ? partialBlock
				: depth <= 1 ? DefinitionAtlas::AIR
				: depth <= 2 ? biome.topBlock
					: depth <= 4 ? biome.soilBlock
						: biome.rockBlock;
		
		if (biomeID != cBiomeID) {
			chunk.biomes.emplace_back(i);
			chunk.biomes.emplace_back(biomeID);
			cBiomeID = biomeID;
		}
		
		if (blockID != cBlockID) {
			chunk.blocks.emplace_back(i);
			chunk.blocks.emplace_back(blockID);
			cBlockID = blockID;
		}
	}
	
	chunk.countRenderableBlocks();
	chunk.generated = true;
}

void MapGen::generateChunkStructures(Job& job, glm::ivec3 localPos, std::vector<unsigned int> biomeMap,
	ChunkData& depthMap) {
	
	glm::vec3 posFloat = job.pos + localPos;
	std::default_random_engine generator(posFloat.x + posFloat.y / M_PI + posFloat.z / (M_PI * 2));
	std::uniform_real_distribution<float> distribution(0, 1);
	
	auto& chunk = job.chunks->at(job.pos + localPos);
	
	for (unsigned short i = 0; i < 256; i++) {
		glm::ivec3 indPos = { i / 16, 0, i % 16 };
		if (distribution(generator) > 0.97) {
			for (indPos.y = 0; indPos.y < 16; indPos.y++) {
				
				unsigned short ind = Space::Block::index(indPos);
				if (depthMap[ind] > 0 && depthMap[ind] <= 1.1) {
					
					glm::ivec3 pos = (job.pos + localPos) * 16 + indPos;
					
					unsigned int biomeID = biomeMap[(localPos.x * 16 + indPos.x) * (job.size * 16 + 1) +
					                                (localPos.z * 16 + indPos.z)];
					auto& biome = game.getBiomes().biomeFromId(biomeID);
					
					auto schematic = biome.schematics.size() > 0 ? biome.schematics[0] : nullptr;
					if (schematic) {
						if (!schematic->processed) schematic->process(game.getDefs());
						for (unsigned int j = 0; j < schematic->length(); j++) {
							glm::ivec3 off = schematic->getOffset(j);
							setBlock(job, pos + off - schematic->origin, schematic->blocks[j], chunk);
						}
					}
				}
			}
		}
	}
}

void MapGen::setBlock(MapGen::Job& job, glm::ivec3 worldPos, unsigned int block, std::shared_ptr<Chunk> hint) {
	if (block == DefinitionAtlas::INVALID) return;
	unsigned int ind = Space::Block::index(worldPos);
	
	if (hint && Space::Chunk::world::fromBlock(worldPos) == hint->getPos()) {
		if (hint->getBlock(ind) <= DefinitionAtlas::AIR) hint->setBlock(ind, block);
	}
	else {
		glm::ivec3 chunkPos = Space::Chunk::world::fromBlock(worldPos);
		auto chunk = *(*job.chunks->emplace(chunkPos, std::make_shared<Chunk>(chunkPos, true)).first).second;
		if (chunk.getBlock(ind) <= DefinitionAtlas::AIR) chunk.setBlock(ind, block);
	}
}

//void MapGen::generateSunlight(MapGen::chunk_partials_map &chunks, glm::ivec3 mbPos) {
//    std::queue<SunlightNode> sunlightQueue;
//
//    glm::ivec3 c {};
//    for (c.x = 0; c.x < 4; c.x++) {
//        for (c.z = 0; c.z < 4; c.z++) {
//            c.y = 3;
//            Chunk* chunk = chunks[mbPos * 4 + c].second;
//
//            glm::ivec3 b {};
//            for (b.x = 0; b.x < 16; b.x++) {
//                for (b.z = 0; b.z < 16; b.z++) {
//                    b.y = 15;
//
//                    while (true) {
//                        unsigned int ind =  Space::Block::index(b);
//                        if (defs.blockFromId(chunk->getBlock(ind)).lightPropagates) {
//                            chunk->setLight(ind, 3, 15);
//                            sunlightQueue.emplace(ind, chunk);
//                        }
//                        else {
//                            c.y = 3;
//                            chunk = chunks[mbPos * 4 + c].second;
//                            break;
//                        }
//
//                        b.y--;
//                        if (b.y < 0) {
//                            b.y = 15;
//                            c.y = c.y ? c.y - 1 : 3;
//                            chunk = chunks[mbPos * 4 + c].second;
//                            if (c.y == 3) break;
//                        }
//                    }
//                }
//            }
//        }
//    }
//
//    propogateSunlightNodes(chunks, sunlightQueue);
//}
//
//bool MapGen::containsWorldPos(Chunk *chunk, glm::ivec3 pos) {
//    return chunk && Space::Chunk::world::fromBlock(pos) == chunk->pos;
//}
//
//void MapGen::propogateSunlightNodes(MapGen::chunk_partials_map &chunks, std::queue<SunlightNode> &queue) {
//    while (!queue.empty()) {
//        SunlightNode& node = queue.front();
//
//        unsigned char lightLevel = node.chunk->getLight(node.index, 3);
//        glm::ivec3 worldPos = node.chunk->pos * 16 + Space::Block::fromIndex(node.index);
//
//        for (const auto& i : Vec::adj) {
//            glm::ivec3 check = worldPos + i;
//
//            Chunk* chunk;
//            if (containsWorldPos(node.chunk, check)) chunk = node.chunk;
//            else {
//                glm::ivec3 worldPos = Space::Chunk::world::fromBlock(check);
//                if (!chunks.count(worldPos)) continue;
//                chunk = chunks[worldPos].second;
//                if (!chunk) continue;
//            }
//
//            auto ind = Space::Block::index(check);
//            if (defs.blockFromId(chunk->getBlock(ind)).lightPropagates && chunk->getLight(ind, 3) + 2 <= lightLevel) {
//                chunk->setLight(ind, 3, lightLevel - static_cast<int>(!(lightLevel == 15 && i.y == -1)));
//                queue.emplace(ind, chunk);
//            }
//        }
//
//        queue.pop();
//    }
//}
//