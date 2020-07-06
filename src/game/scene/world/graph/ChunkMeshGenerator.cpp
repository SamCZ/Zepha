//
// Created by aurailus on 01/12/18.
//

#include <vector>
#include <glm/gtx/normal.hpp>

#include "ChunkMeshGenerator.h"

#include "../ChunkMeshDetails.h"
#include "../../../../util/Vec.h"
#include "../../../../world/chunk/Chunk.h"
#include "../../../../def/item/BlockModel.h"
#include "../../../../def/gen/NoiseSample.h"
#include "../../../../def/gen/LocalBiomeAtlas.h"
#include "../../../../def/LocalDefinitionAtlas.h"

ChunkMeshGenerator::ChunkMeshGenerator(ChunkMeshDetails* meshDetails, LocalDefinitionAtlas& defs, LocalBiomeAtlas& biomes,
    std::shared_ptr<Chunk> chunk, std::array<std::shared_ptr<Chunk>, 6> adjacent,
    std::array<NoiseSample, 3>& blockOffsets) :

    defs(defs),
    chunk(chunk),
    biomes(biomes),
    adjacent(adjacent),
    meshDetails(meshDetails) {

    meshDetails->vertices.reserve(5000);
    meshDetails->indices.reserve(7000);

    // Lock the related chunks
    std::array<std::unique_lock<std::mutex>, 7> locks;
    locks[0] = std::move(chunk->aquireLock());
    for (unsigned int i = 0; i < 6; i++) locks[i+1] = std::move(adjacent[i]->aquireLock());

    const auto& blockData = chunk->cGetBlocks();
    const auto& biomeData = chunk->cGetBiomes();

    unsigned short blockArrayPos = 0;
    unsigned int   cBlock = blockData[blockArrayPos + 1];
    unsigned short biomeArrayPos = 0;
    unsigned short cBiome = biomeData[biomeArrayPos + 1];

    glm::ivec3 off;
    for (unsigned short i = 0; i < 4096; i++) {
        if (blockArrayPos + 2 < blockData.size() && i >= blockData[blockArrayPos + 2]) {
            blockArrayPos += 2;
            cBlock = blockData[blockArrayPos + 1];
        }
        if (biomeArrayPos + 2 < biomeData.size() && i >= biomeData[biomeArrayPos + 2]) {
            biomeArrayPos += 2;
            cBiome = biomeData[biomeArrayPos + 1];
        }

        BlockModel& model = defs.blockFromId(cBlock).model;
        glm::vec3 biomeTint = biomes.biomeFromId(cBiome).biomeTint;

        if (!model.visible) continue;

        Vec::indAssignVec(i, off);
        glm::vec3 vis = off;

        for (auto& mod : model.meshMods) {
            switch (mod.first) {
                default: break;

                case MeshMod::OFFSET_X:
                    vis.x += blockOffsets[0].get(off) * mod.second; break;
                case MeshMod::OFFSET_Y:
                    vis.y += blockOffsets[1].get(off) * mod.second; break;
                case MeshMod::OFFSET_Z:
                    vis.z += blockOffsets[2].get(off) * mod.second; break;
            }
        }

        glm::ivec3 pos {};
        pos = { off.x - 1, off.y, off.z };
        if (!getBlockAt(pos).culls) addFaces(vis, model.parts[static_cast<int>(Dir::XNEG)], biomeTint, getLightAt(pos));
        pos = { off.x + 1, off.y, off.z };
        if (!getBlockAt(pos).culls) addFaces(vis, model.parts[static_cast<int>(Dir::XPOS)], biomeTint, getLightAt(pos));
        pos = { off.x, off.y - 1, off.z };
        if (!getBlockAt(pos).culls) addFaces(vis, model.parts[static_cast<int>(Dir::YNEG)], biomeTint, getLightAt(pos));
        pos = { off.x, off.y + 1, off.z };
        if (!getBlockAt(pos).culls) addFaces(vis, model.parts[static_cast<int>(Dir::YPOS)], biomeTint, getLightAt(pos));
        pos = { off.x, off.y, off.z - 1 };
        if (!getBlockAt(pos).culls) addFaces(vis, model.parts[static_cast<int>(Dir::ZNEG)], biomeTint, getLightAt(pos));
        pos = { off.x, off.y, off.z + 1 };
        if (!getBlockAt(pos).culls) addFaces(vis, model.parts[static_cast<int>(Dir::ZPOS)], biomeTint, getLightAt(pos));

        addFaces(vis, model.parts[static_cast<int>(Dir::NO_CULL)], biomeTint, getLightAt(vis));
    }

    meshDetails->vertices.shrink_to_fit();
    meshDetails->indices.shrink_to_fit();
}

BlockDef& ChunkMeshGenerator::getBlockAt(const glm::ivec3& pos) {
    if (pos.x == -1) return defs.blockFromId(adjacent[0]->getBlock(pos + glm::ivec3 {16, 0, 0}));
    if (pos.x == 16) return defs.blockFromId(adjacent[1]->getBlock(pos - glm::ivec3 {16, 0, 0}));

    if (pos.y == -1) return defs.blockFromId(adjacent[2]->getBlock(pos + glm::ivec3 {0, 16, 0}));
    if (pos.y == 16) return defs.blockFromId(adjacent[3]->getBlock(pos - glm::ivec3 {0, 16, 0}));

    if (pos.z == -1) return defs.blockFromId(adjacent[4]->getBlock(pos + glm::ivec3 {0, 0, 16}));
    if (pos.z == 16) return defs.blockFromId(adjacent[5]->getBlock(pos - glm::ivec3 {0, 0, 16}));

    return defs.blockFromId(chunk->getBlock(pos));
}

glm::vec4 ChunkMeshGenerator::getLightAt(const glm::ivec3& pos) {
    if (pos.x == -1) return adjacent[0]->getLight(Space::Block::index(pos + glm::ivec3 {16, 0, 0}));
    if (pos.x == 16) return adjacent[1]->getLight(Space::Block::index(pos - glm::ivec3 {16, 0, 0}));

    if (pos.y == -1) return adjacent[2]->getLight(Space::Block::index(pos + glm::ivec3 {0, 16, 0}));
    if (pos.y == 16) return adjacent[3]->getLight(Space::Block::index(pos - glm::ivec3 {0, 16, 0}));

    if (pos.z == -1) return adjacent[4]->getLight(Space::Block::index(pos + glm::ivec3 {0, 0, 16}));
    if (pos.z == 16) return adjacent[5]->getLight(Space::Block::index(pos - glm::ivec3 {0, 0, 16}));

    return chunk->getLight(Space::Block::index(pos));
}

void ChunkMeshGenerator::addFaces(const glm::vec3 &offset, const std::vector<MeshPart> &meshParts, const glm::vec3& tint, glm::vec4 light) {
    for (const MeshPart& mp : meshParts) {
        glm::vec3 modData = {};

        switch (mp.shaderMod) {
            default: break;

            case ShaderMod::ROTATE_X:
            case ShaderMod::ROTATE_Y:
            case ShaderMod::ROTATE_Z:
            case ShaderMod::SWAY_ATTACHED:
            case ShaderMod::SWAY_FULL_BLOCK:
                modData = { Util::packFloat((offset - 8.f) / 8.f), mp.modValue, 0 };
                break;
        }

        for (const BlockModelVertex &vertex : mp.vertices) {
            meshDetails->vertices.push_back({
                vertex.pos + offset,
                vertex.tex,
                mp.blendInd ? tint : glm::vec3 {1, 1, 1},
                mp.blendInd ? vertex.blendMask : glm::vec2 {-1, -1},
                Util::packFloat(vertex.nml),
                glm::vec4(light),
                static_cast<float>(mp.shaderMod),
                modData
            });
        }

        for (unsigned int index : mp.indices) {
            meshDetails->indices.push_back(indOffset + index);
        }

        indOffset += mp.vertices.size();
    }
}