//
// Created by aurailus on 01/12/18.
//

#pragma once

#define GLM_ENABLE_EXPERIMENTAL

#include <memory>

class LocalDefinitionAtlas;
class ChunkMeshDetails;
class LocalBiomeAtlas;
class NoiseSample;
class BlockDef;
class MeshPart;
class Chunk;

class ChunkMeshGenerator {
public:
    ChunkMeshGenerator(ChunkMeshDetails* meshDetails, LocalDefinitionAtlas& defs, LocalBiomeAtlas& biomes,
        std::shared_ptr<Chunk> chunk, std::array<std::shared_ptr<Chunk>, 6> adjacent,
        std::array<NoiseSample, 3>& blockOffsets);
private:
    inline BlockDef& getBlockAt(const glm::ivec3& pos);
    inline glm::vec4 getLightAt(const glm::ivec3& pos);

    void addFaces(const glm::vec3 &offset, const std::vector<MeshPart> &meshParts, const glm::vec3& tint, glm::vec4 light);

    LocalDefinitionAtlas& defs;
    LocalBiomeAtlas& biomes;

    unsigned int indOffset = 0;
    ChunkMeshDetails* meshDetails;

    std::shared_ptr<Chunk> chunk;
    std::array<std::shared_ptr<Chunk>, 6> adjacent;
};