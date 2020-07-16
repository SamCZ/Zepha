//
// Created by aurailus on 28/01/19.
//

#include <random>

#include "MapGen.h"

#include "BiomeDef.h"
#include "BiomeAtlas.h"
#include "MapGenProps.h"
#include "NoiseSample.h"
#include "../item/BlockDef.h"
#include "../DefinitionAtlas.h"
#include "../../world/chunk/Chunk.h"
#include "../../game/scene/world/Schematic.h"

MapGen::MapGen(DefinitionAtlas& defs, BiomeAtlas& biomes, unsigned int seed) :
    defs(defs),
    biomes(biomes),
    props(seed) {}

std::unique_ptr<MapGen::ChunkMap> MapGen::generateChunk(glm::ivec3 pos) {
    return generateArea(pos);
}

std::unique_ptr<MapGen::ChunkMap> MapGen::generateMapBlock(glm::ivec3 pos) {
    return generateArea(Space::Chunk::world::fromMapBlock(pos), 4);
}

std::unique_ptr<MapGen::ChunkMap> MapGen::generateArea(glm::ivec3 origin, unsigned int size) {
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
        glm::vec3 indPos = { i / (job.size * 16 + 1), 0, i % (job.size * 16 + 1)};
        glm::vec3 queryPos = indPos / 16.f / static_cast<float>(job.size);

        biomeMap[i] = this->biomes.getBiomeAt(job.temperature.get(queryPos),
            job.humidity.get(queryPos), job.roughness.get(queryPos)).index;
    }

    // Generate Heightmap and Volume

    job.heightmap.populate([&](glm::vec3 pos) {
        glm::ivec3 blockPos = glm::ivec3(pos * 16.f * static_cast<float>(job.size));
        auto& biome = biomes.biomeFromId(biomeMap.at(blockPos.x * (job.size * 16 + 1) + blockPos.z));
        glm::vec3 worldPos = glm::vec3(job.pos) + pos * static_cast<float>(job.size);
        return biome.heightmap[biome.heightmap.size() - 1]->GetValue(worldPos.x, 0, worldPos.z);
    });

    job.volume.populate([&](glm::vec3 pos) {
        glm::ivec3 blockPos = glm::ivec3(pos * 16.f * static_cast<float>(job.size));
        auto& biome = biomes.biomeFromId(biomeMap.at(blockPos.x * (job.size * 16 + 1) + blockPos.z));
        glm::vec3 worldPos = glm::vec3(job.pos) + pos * static_cast<float>(job.size);
        return biome.volume[biome.volume.size() - 1]->GetValue(worldPos.x, worldPos.y, worldPos.z);
    });

    // Generate Chunks

	glm::ivec3 pos {};
    for (pos.x = 0; pos.x < job.size; pos.x++)
        for (pos.z = 0; pos.z < job.size; pos.z++) {
            std::unique_ptr<ChunkData> densityAbove = nullptr;
            for (pos.y = job.size; pos.y >= 0; pos.y--) {
                if (pos.y == job.size) { densityAbove = populateChunkDensity(job, pos); continue; }

                std::unique_ptr<ChunkData> density = populateChunkDensity(job, pos);
                std::unique_ptr<ChunkData> depth = populateChunkDepth(job, pos, density, std::move(densityAbove));

                populateChunk(job, pos, biomeMap, *depth);

                densityAbove = std::move(density);
            }
        }

//    generateSunlight(chunks, mbPos);

	return std::move(job.chunks);
}

std::unique_ptr<MapGen::ChunkData> MapGen::populateChunkDensity(MapGen::Job &job, glm::ivec3 localPos) {
    auto data = std::make_unique<ChunkData>();

    for (int i = 0; i < 4096; i++) {
        glm::ivec3 indPos = Space::Block::fromIndex(i);
        glm::vec3 queryPos = (glm::vec3(localPos) + glm::vec3(indPos) / 16.f) / static_cast<float>(job.size);
        (*data)[i] = (job.volume.get(queryPos) + job.heightmap.get({queryPos.x, 0, queryPos.z})) - ((job.pos.y + localPos.y) * 16 + indPos.y);
    }

    return data;
}

std::unique_ptr<MapGen::ChunkData> MapGen::populateChunkDepth(Job& job, glm::ivec3 localPos, std::unique_ptr<ChunkData>& chunkDensity, std::unique_ptr<ChunkData> chunkDensityAbove) {
    auto data = std::make_unique<ChunkData>();

    for (unsigned short i = 0; i < 256; i++) {
        glm::ivec2 pos = { i / 16, i % 16 };
        short depth = 16;

        if ((*chunkDensity)[Space::Block::index({pos.x, 15, pos.y})] > 0) {
            for (unsigned char j = 0; j < 16; j++) {
                if ((*chunkDensityAbove)[Space::Block::index({pos.x, j, pos.y})] <= 0) {
                    depth = j;
                    break;
                }
            }
        }
        else {
            depth = 0;
        }

        for (char y = 15; y >= 0; y--) {
            unsigned int ind = Space::Block::index({pos.x, y, pos.y});
            depth = ((*chunkDensity)[ind] > 0 ? std::min(depth + 1, 16) : 0);
            (*data)[ind] = depth + ((*chunkDensity)[ind] - static_cast<int>((*chunkDensity)[ind]));
        }
    }

    return data;
}

void MapGen::populateChunk(Job& job, glm::ivec3 localPos, std::vector<unsigned int> biomeMap, ChunkData& depthMap) {
    glm::ivec3 chunkPos = job.pos + localPos;
    auto& chunk = *(*job.chunks->emplace(chunkPos, std::make_shared<Chunk>(chunkPos)).first).second;

    unsigned int cBlockID = -1;
    unsigned int cBiomeID = -1;

    for (unsigned short i = 0; i < 4096; i++) {
        glm::ivec3 indPos = Space::Block::fromIndex(i);

        unsigned int biomeID = biomeMap[(localPos.x * 16 + indPos.x) * (job.size * 16 + 1) + (localPos.z * 16 + indPos.z)];
        auto& biome = this->biomes.biomeFromId(biomeID);

        float depth = depthMap[i];
        unsigned int blockID
            = depth <= 1 ? DefinitionAtlas::AIR
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

//void MapGen::generateStructures(chunk_partials_map& chunks, chunk_partial& chunk) {
//    std::default_random_engine generator(chunk.second->pos.x + chunk.second->pos.y * 30 + chunk.second->pos.z * 3.5);
//    std::uniform_real_distribution<float> distribution(0, 1);
//
//    glm::ivec3 wp = chunk.second->pos;
//    glm::ivec3 lp;
//
//    for (unsigned short i = 0; i < 256; i++) {
//        unsigned short x = i / 16;
//        unsigned short z = i % 16;
//
//        if (distribution(generator) > 0.97) {
//            for (unsigned short y = 0; y < 16; y++) {
//                lp = {x, y, z};
//                unsigned short ind = Space::Block::index(lp);
//
//                if (chunk.first->depth[ind] > 0 && chunk.first->depth[ind] <= 1.1) {
//
//                    glm::ivec3 off = {};
//                    glm::ivec3 p = wp * 16 + lp;
//
//                    auto biome = biomes.biomeFromId(chunk.second->getBiome(ind));
//                    auto schematic = biome.schematics.size() > 0 ? biome.schematics[0] : nullptr;
//
//                    if (schematic != nullptr) {
//                        if (!schematic->processed) schematic->process(defs);
//                        for (unsigned int j = 0; j < schematic->length(); j++) {
//                            schematic->assignOffset(j, off);
//                            setBlock(p + off - schematic->origin, schematic->blocks[j], chunks);
//                        }
//                    }
//                }
//            }
//        }
//    }
//}
//
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
//void MapGen::setBlock(glm::ivec3 worldPos, unsigned int block, MapGen::chunk_partials_map &chunks) {
//    if (block == DefinitionAtlas::INVALID) return;
//
//    glm::ivec3 chunkPos = Space::Chunk::world::fromBlock(worldPos);
//    Chunk* chunk = nullptr;
//
//    if (chunks.count(chunkPos)) chunk = chunks.at(chunkPos).second;
//    else {
//        chunk = new Chunk();
//        chunk->pos = chunkPos;
//        chunk->partial = true;
//        chunks.insert(std::pair<glm::ivec3, chunk_partial>{chunkPos, {new MapGenJob(), chunk}});
//    }
//
//    unsigned int index = Space::Block::index(worldPos);
//    if (chunk->getBlock(index) <= DefinitionAtlas::AIR) chunk->setBlock(index, block);
//}
//
//std::shared_ptr<Chunk> MapGen::combinePartials(std::shared_ptr<Chunk> a, std::shared_ptr<Chunk> b) {
//    std::shared_ptr<Chunk> src;
//    std::shared_ptr<Chunk> res;
//
//    if (a->generated) {
//        res = a;
//        src = b;
//    }
//    else {
//        res = b;
//        src = a;
//    }
//
//    for (unsigned int i = 0; i < 4096; i++) {
//        if (src->getBlock(i) > DefinitionAtlas::INVALID) res->setBlock(i, src->getBlock(i));
//    }
//
//    res->generated = src->generated || res->generated;
//    res->partial = !res->generated;
//    res->recalculateRenderableBlocks();
//    return res;
//}
