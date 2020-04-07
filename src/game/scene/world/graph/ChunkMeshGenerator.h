//
// Created by aurailus on 01/12/18.
//

#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define GLM_ENABLE_EXPERIMENTAL
#pragma clang diagnostic pop

#include "../../../../def/ClientGame.h"
#include "../../../../def/gen/NoiseSample.h"

class ChunkMeshDetails;
class BlockChunk;

class ChunkMeshGenerator {
public:
    ChunkMeshGenerator(ChunkMeshDetails* meshDetails, LocalDefinitionAtlas& defs, LocalBiomeAtlas& biomes,
                       std::shared_ptr<BlockChunk> chunk, std::array<std::shared_ptr<BlockChunk>, 6> adjacent, std::array<NoiseSample, 3>& blockOffsets);
private:
    BlockDef& getBlockAt(const glm::ivec3 &pos);
    void addFaces(const glm::vec3 &offset, const std::vector<MeshPart> &meshParts, const glm::vec3& tint);

    LocalDefinitionAtlas& defs;
    LocalBiomeAtlas& biomes;

    unsigned int indOffset = 0;
    ChunkMeshDetails* meshDetails;

    std::shared_ptr<BlockChunk> chunk;
    std::array<std::shared_ptr<BlockChunk>, 6> adjacent;
};