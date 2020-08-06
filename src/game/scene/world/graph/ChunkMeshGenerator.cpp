//
// Created by aurailus on 01/12/18.
//

#include <vector>
#include <glm/gtx/normal.hpp>

#include "ChunkMeshGenerator.h"

#include "../ChunkMeshDetails.h"
#include "../../../../util/Vec.h"
#include "../../../../util/Util.h"
#include "../../../../def/gen/BiomeDef.h"
#include "../../../../world/chunk/Chunk.h"
#include "../../../../def/item/BlockDef.h"
#include "../../../../def/item/BlockModel.h"
#include "../../../../def/gen/NoiseSample.h"
#include "../../../../def/gen/LocalBiomeAtlas.h"
#include "../../../../def/LocalDefinitionAtlas.h"
#include "../../../../util/Timer.h"

ChunkMeshGenerator::ChunkMeshGenerator(ChunkMeshDetails* meshDetails, LocalDefinitionAtlas& defs, LocalBiomeAtlas& biomes,
    std::shared_ptr<Chunk> chunk, std::array<std::shared_ptr<Chunk>, 6> adjacent, std::array<NoiseSample, 3>& blockOffsets) :
    meshDetails(meshDetails),
    adjacent(adjacent),
    biomes(biomes),
    chunk(chunk),
    defs(defs) {

    Timer t("Mesh generation");

    meshDetails->vertices.reserve(5000);
    meshDetails->indices.reserve(7000);

    auto l = chunk->getReadLock();
    RIE::expand<unsigned int, 4096>(chunk->cGetBlocks(), eBlocks);
    RIE::expand<unsigned short, 4096>(chunk->cGetBiomes(), eBiomes);
    l.unlock();

    BlockDef* block = nullptr;
    BiomeDef* biome = nullptr;

    for (unsigned short i = 0; i < 4096; i++) {
        if (!block || block->index != eBlocks[i]) block = &defs.blockFromId(eBlocks[i]);
        if (!biome || biome->index != eBiomes[i]) biome = &biomes.biomeFromId(eBiomes[i]);

        BlockModel& model = block->model;
        glm::vec3 biomeTint = biome->tint;

        if (!model.visible) continue;

        glm::ivec3 off = Space::Block::fromIndex(i);
        glm::vec3 vis = off;

        for (auto& mod : model.meshMods) {
            switch (mod.first) {
                default: break;
                case MeshMod::OFFSET_X: vis.x += blockOffsets[0].get(vis / 16.f) * mod.second; break;
                case MeshMod::OFFSET_Y: vis.y += blockOffsets[1].get(vis / 16.f) * mod.second; break;
                case MeshMod::OFFSET_Z: vis.z += blockOffsets[2].get(vis / 16.f) * mod.second; break;
            }
        }

        glm::ivec3 pos = { off.x - 1, off.y, off.z };
        if (!getBlockAt(pos).culls) addFaces(vis, model.parts[static_cast<int>(EVec::XNEG)], biomeTint, getLightAt(pos));
        pos = { off.x + 1, off.y, off.z };
        if (!getBlockAt(pos).culls) addFaces(vis, model.parts[static_cast<int>(EVec::XPOS)], biomeTint, getLightAt(pos));
        pos = { off.x, off.y - 1, off.z };
        if (!getBlockAt(pos).culls) addFaces(vis, model.parts[static_cast<int>(EVec::YNEG)], biomeTint, getLightAt(pos));
        pos = { off.x, off.y + 1, off.z };
        if (!getBlockAt(pos).culls) addFaces(vis, model.parts[static_cast<int>(EVec::YPOS)], biomeTint, getLightAt(pos));
        pos = { off.x, off.y, off.z - 1 };
        if (!getBlockAt(pos).culls) addFaces(vis, model.parts[static_cast<int>(EVec::ZNEG)], biomeTint, getLightAt(pos));
        pos = { off.x, off.y, off.z + 1 };
        if (!getBlockAt(pos).culls) addFaces(vis, model.parts[static_cast<int>(EVec::ZPOS)], biomeTint, getLightAt(pos));

        addFaces(vis, model.parts[static_cast<int>(EVec::NO_CULL)], biomeTint, getLightAt(vis));
    }

    meshDetails->vertices.shrink_to_fit();
    meshDetails->indices.shrink_to_fit();

//    t.printElapsedMs();
}

BlockDef& ChunkMeshGenerator::getBlockAt(const glm::ivec3& pos) {
    glm::ivec3 dir = {(pos.x < 0 ? -1 : pos.x > 15 ? 1 : 0),
        (pos.y < 0 ? -1 : pos.y > 15 ? 1 : 0), (pos.z < 0 ? -1 : pos.z > 15 ? 1 : 0)};

    if (dir != glm::ivec3 {0, 0, 0}) {
        unsigned int ind = static_cast<unsigned int>(Vec::TO_ENUM.at(dir));
        auto& chunk = adjacent[ind];
        return defs.blockFromId(chunk->getBlock(Space::Block::index(pos - dir * 16)));
    }

    return defs.blockFromId(eBlocks[Space::Block::index(pos)]);
}

glm::vec4 ChunkMeshGenerator::getLightAt(const glm::ivec3& pos) {
    glm::ivec3 dir = {(pos.x < 0 ? -1 : pos.x > 15 ? 1 : 0),
         (pos.y < 0 ? -1 : pos.y > 15 ? 1 : 0), (pos.z < 0 ? -1 : pos.z > 15 ? 1 : 0)};

    if (dir != glm::ivec3 {0, 0, 0}) {
        unsigned int ind = static_cast<unsigned int>(Vec::TO_ENUM.at(dir));
        auto& chunk = adjacent[ind];
        return chunk->getLight(Space::Block::index(pos - dir * 16));
    }

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