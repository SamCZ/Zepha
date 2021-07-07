#include <random>

#include "MapGen.h"

#include "world/World.h"
#include "game/Subgame.h"
#include "util/Structure.h"
#include "game/def/BiomeDef.h"
#include "game/def/BlockDef.h"
#include "world/dim/Dimension.h"
#include "game/atlas/BiomeAtlas.h"
#include "world/dim/chunk/Chunk.h"
#include "game/atlas/DefinitionAtlas.h"

MapGen::MapGen(Subgame& game, World& world, u32 seed, std::unordered_set<string> biomes) :
	game(game), world(world), props(seed) {
	
	std::unordered_set<u16> biomeIndices {};
	for (const auto& str : biomes) {
		if (str[0] == '#')
			for (auto& biome : game.getBiomes().biomesFromTag(str.substr(1, str.length() - 1)))
				biomeIndices.insert(biome->index);
		else biomeIndices.insert(game.getBiomes().biomeFromStr(str).index);
	}
	
	generateVoronoi(biomeIndices);
}

[[maybe_unused]] uptr<MapGen::ChunkMap> MapGen::generateChunk(u16 dim, ivec3 pos) {
	return generateArea(dim, pos, 1);
}

uptr<MapGen::ChunkMap> MapGen::generateMapBlock(u16 dim, ivec3 pos) {
	return generateArea(dim, Space::Chunk::world::fromMapBlock(pos), 4);
}

std::unique_ptr<MapGen::ChunkMap> MapGen::generateArea(u16 dim, ivec3 origin, u16 size) {
	Job job(origin, size);

	// Build Biome Prop Maps

	const auto fill = [&](const noise::module::Module& s) {
		return [&](vec3 pos) {
			vec3 worldPos = vec3(job.pos) + pos * static_cast<f32>(job.size);
			return s.GetValue(worldPos.x, 0, worldPos.z);
		};
	};

	job.temperature.populate(fill(props.temperature));
	job.roughness.populate(fill(props.roughness));
	job.humidity.populate(fill(props.humidity));

	// Generate Biome Topmap

	vec<u16> biomeMap {};
	biomeMap.resize((job.size * 16 + 1) * (job.size * 16 + 1));

	for (usize i = 0; i < biomeMap.size(); i++) {
		vec3 indPos = { i / (job.size * 16 + 1), 0, i % (job.size * 16 + 1) };
		vec3 queryPos = indPos / 16.f / static_cast<f32>(job.size);

		biomeMap[i] = getBiomeAt(job.temperature.get(queryPos),
			job.humidity.get(queryPos), job.roughness.get(queryPos));
	}

	// Generate Heightmap and Volume

	job.heightmap.populate([&](vec3 pos) {
		ivec3 blockPos = ivec3(pos * 16.f * static_cast<f32>(job.size));
		auto& biome = game.getBiomes().biomeFromId(biomeMap.at(blockPos.x * (job.size * 16 + 1) + blockPos.z));
		vec3 worldPos = vec3(job.pos) + pos * static_cast<f32>(job.size);
		return biome.heightmap[biome.heightmap.size() - 1]->GetValue(worldPos.x, 0, worldPos.z);
	});

	job.volume.populate([&](vec3 pos) {
		ivec3 blockPos = ivec3(pos * 16.f * static_cast<f32>(job.size));
		auto& biome = game.getBiomes().biomeFromId(biomeMap.at(blockPos.x * (job.size * 16 + 1) + blockPos.z));
		vec3 worldPos = vec3(job.pos) + pos * static_cast<f32>(job.size);
		return biome.volume[biome.volume.size() - 1]->GetValue(worldPos.x, worldPos.y, worldPos.z);
	});

	// Generate Chunks

	i16vec3 pos {};
	for (pos.x = 0; pos.x < job.size; pos.x++) {
		for (pos.z = 0; pos.z < job.size; pos.z++) {
			uptr<ChunkData> densityAbove = nullptr;
			for (pos.y = job.size; pos.y >= 0; pos.y--) {
				if (pos.y == job.size) {
					densityAbove = populateChunkDensity(job, pos);
					continue;
				}

				uptr<ChunkData> density = populateChunkDensity(job, pos);
				uptr<ChunkData> depth = populateChunkDepth(density, std::move(densityAbove));

				generateChunkBlocks(job, pos, biomeMap, *depth);
//				generateChunkDecorAndLight(job, pos, biomeMap, *depth);

				densityAbove = std::move(density);
			}
		}
	}

	return std::move(job.chunks);
}

void MapGen::generateVoronoi(const std::unordered_set<u16>& biomes) {
	vec<std::pair<vec3, u16>> points {};
	for (auto biomeInd : biomes) {
		auto& biome = game.getBiomes().biomeFromId(biomeInd);
		
		points.emplace_back(vec3 {
			static_cast<u16>(std::fmin(voronoiSize - 1,
				std::fmax(0, (biome.temperature + 1) / 2 * voronoiSize))),
			static_cast<u16>(std::fmin(voronoiSize - 1, std::fmax(0, biome.humidity * voronoiSize))),
			static_cast<u16>(std::fmin(voronoiSize - 1, std::fmax(0, biome.roughness * voronoiSize)))
		}, biomeInd);
	}
	
	voronoi.setPoints(points);
}

u16 MapGen::getBiomeAt(f32 temperature, f32 humidity, f32 roughness) {
	return voronoi.getPoint(
		static_cast<u16>(std::fmin(voronoiSize - 1, std::fmax(0, (temperature + 1) / 2 * voronoiSize))),
		static_cast<u16>(std::fmin(voronoiSize - 1, std::fmax(0, humidity * voronoiSize))),
		static_cast<u16>(std::fmin(voronoiSize - 1, std::fmax(0, roughness * voronoiSize))));
}

uptr<MapGen::ChunkData> MapGen::populateChunkDensity(MapGen::Job& job, ivec3 localPos) {
	auto data = make_unique<ChunkData>();
	
	for (u16 i = 0; i < 4096; i++) {
		ivec3 indPos = Space::Block::fromIndex(i);
		vec3 queryPos = (vec3(localPos) + vec3(indPos) / 16.f) / static_cast<f32>(job.size);
		(*data)[i] = (job.volume.get(queryPos) + job.heightmap.get({ queryPos.x, 0, queryPos.z }))
			- ((job.pos.y + localPos.y) * 16 + indPos.y);
	}
	
	return data;
}

uptr<MapGen::ChunkData> MapGen::populateChunkDepth(uptr<ChunkData>& chunkDensity, uptr<ChunkData> chunkDensityAbove) {
	auto data = make_unique<ChunkData>();
	
	for (u16 i = 0; i < 256; i++) {
		ivec2 pos = { i / 16, i % 16 };
		short depth = 16;

		if ((*chunkDensity)[Space::Block::index({ pos.x, 15, pos.y })] > 0) {
			for (u8 j = 0; j < 16; j++) {
				if ((*chunkDensityAbove)[Space::Block::index({ pos.x, j, pos.y })] <= 0) {
					depth = j;
					break;
				}
			}
		}
		else {
			depth = 0;
		}

		for (i8 y = 15; y >= 0; y--) {
			u16 ind = Space::Block::index({ pos.x, y, pos.y });
			depth = ((*chunkDensity)[ind] > 0 ? std::min(depth + 1, 16) : 0);
			(*data)[ind] = depth;
		}
	}
	
	return data;
}

void MapGen::generateChunkBlocks(Job& job, ivec3 localPos, vec<u16> biomeMap, ChunkData& depthMap) {
	ivec3 chunkPos = job.pos + localPos;
	
	auto partial = (job.chunks->count(chunkPos) ? job.chunks->at(chunkPos) : nullptr);
	if (partial) job.chunks->erase(chunkPos);
	
	auto& chunk = *(*job.chunks->emplace(chunkPos, new Chunk(chunkPos)).first).second;
	
	u16 partialBlock = DefinitionAtlas::INVALID;
	
	for (u16 i = 0; i < 4096; i++) {
		ivec3 indPos = Space::Block::fromIndex(i);
		
		u16 biomeId = biomeMap[(localPos.x * 16 + indPos.x) * (job.size * 16 + 1) + (localPos.z * 16 + indPos.z)];
		auto& biome = game.getBiomes().biomeFromId(biomeId);
		chunk.d->biomes[i] = biomeId;
		
		f32 depth = depthMap[i];
		u16 blockId =
			partialBlock > DefinitionAtlas::INVALID ? partialBlock
				: depth <= 1 ? DefinitionAtlas::AIR
				: depth <= 2 ? biome.topBlock
				: depth <= 4 ? biome.soilBlock
				: biome.rockBlock;
		
		if (chunk.d == nullptr) std::cout << "THE DATA ISNT LOADED." << std::endl;
		chunk.d->blocks[i] = blockId;
		
	}
	
	chunk.countRenderableBlocks();
}

void MapGen::generateChunkDecorAndLight(Job& job, ivec3 localPos, vec<u16> biomeMap,
	ChunkData& depthMap) {
	
	vec3 posFloat = job.pos + localPos;
	std::default_random_engine generator(posFloat.x + posFloat.y * M_PI + posFloat.z * (M_PI * 2));
	std::uniform_real_distribution<f32> distribution(0, 1);
	
	auto& chunk = job.chunks->at(job.pos + localPos);
	
	ivec3 abovePos = job.pos + localPos + ivec3 { 0, 1, 0 };
	Chunk* above = (localPos.y != job.size - 1) ?
		job.chunks->count(abovePos) ? job.chunks->at(abovePos) : nullptr : nullptr;
	
	for (u16 i = 0; i < 256; i++) {
		ivec3 indPos = { i / 16, 15, i % 16 };
		
		u16 biomeID = biomeMap[(localPos.x * 16 + indPos.x)
			* (job.size * 16 + 1) + (localPos.z * 16 + indPos.z)];
		auto& biome = game.getBiomes().biomeFromId(biomeID);
		
		u16 schemID = -1;
		for (u16 j = 0; j < biome.schematics.size(); j++) {
			if (distribution(generator) > 1 - biome.schematics[j]->probability) {
				schemID = j;
				break;
			}
		}
		
		i8 light = -1;
		
		for (; indPos.y >= 0 && (light || schemID > -1); indPos.y--) {
			u16 ind = Space::Block::index(indPos);
			
			if (schemID > UINT16_MAX && depthMap[ind] > 1 && depthMap[ind] <= 2) {
				ivec3 pos = (job.pos + localPos) * 16 + indPos;
				pos.y++; // Compensate for the fact that we're finding solid positions.
				auto& schematic = biome.schematics[schemID];
				for (usize j = 0; j < schematic->length(); j++) {
					ivec3 off = schematic->getOffset(j);
					setBlock(job, pos + off - schematic->origin, schematic->layout[j], chunk);
				}
				break;
			}
			
			if (light == -1) light = above ? above->getLight(Space::Block::index(indPos), 3) :
				game.getDefs().blockFromId(chunk->getBlock(indPos)).lightPropagates ? 15 : 0;
			
			if (!light) continue;
			
			auto& blockDef = game.getDefs().blockFromId(chunk->getBlock(indPos));
			if (!blockDef.lightPropagates) light = 0;
			else {
				chunk->setLight(ind, 3, light);
				job.sunlightQueue.emplace(ind, chunk);
			}
		}
	}
	
	chunk->generationState = Chunk::GenerationState::GENERATED;
}

void MapGen::setBlock(MapGen::Job& job, ivec3 worldPos, u16 block, Chunk* hint) {
	if (block == DefinitionAtlas::INVALID) return;
	u16 ind = Space::Block::index(worldPos);
	
	if (hint && Space::Chunk::world::fromBlock(worldPos) == hint->getPos()) {
		if (hint->getBlock(ind) <= DefinitionAtlas::AIR) hint->setBlock(ind, block);
	}
	else {
		ivec3 chunkPos = Space::Chunk::world::fromBlock(worldPos);
		auto& chunk = *(*job.chunks->emplace(chunkPos, new Chunk(chunkPos, true)).first).second;
		if (chunk.getBlock(ind) <= DefinitionAtlas::AIR) chunk.setBlock(ind, block);
	}
}

void MapGen::propogateSunlightNodes(Job& job) {
//	auto& defs = game.getDefs();
//
//	while (!job.sunlightQueue.empty()) {
//		SunlightNode& node = job.sunlightQueue.front();
//
//		unsigned char lightLevel = node.chunk->getLight(node.index, 3);
//		glm::ivec3 worldPos = node.chunk->pos * 16 + Space::Block::fromIndex(node.index);
//
//		for (const auto& i : Vec::TO_VEC) {
//			glm::ivec3 check = worldPos + i;
//
//			Chunk* chunk;
//			glm::ivec3 chunkPos = Space::Chunk::world::fromBlock(check);
//			if (node.chunk->pos == chunkPos) chunk = node.chunk;
//			else {
//				auto found = job.chunks->find(chunkPos);
//				if (found == job.chunks->end()) continue;
//				chunk = found->second.get();
//			}
//
//			auto ind = Space::Block::index(check);
//			if (defs.blockFromId(chunk->getBlock(ind)).lightPropagates && chunk->getLight(ind, 3) + 2 <= lightLevel) {
//				chunk->setLight(ind, 3, lightLevel - static_cast<int>(!(lightLevel == 15 && i.y == -1)));
//				job.sunlightQueue.emplace(ind, chunk);
//			}
//		}
//
//		job.sunlightQueue.pop();
//	}
}
