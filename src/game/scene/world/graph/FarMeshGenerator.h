//
// Created by aurailus on 2020-04-06.
//

#pragma once

#pragma clang diagnostic push
#pragma ide diagnostic ignored "OCUnusedMacroInspection"
#define GLM_ENABLE_EXPERIMENTAL
#pragma clang diagnostic pop

#include "../../../../def/ClientGame.h"
#include "../../../../def/gen/NoiseSample.h"

class FarMapMeshDetails;

//class FarMeshGenerator {
//public:
//    FarMeshGenerator(LocalDefinitionAtlas& defs, LocalBiomeAtlas& biomes,
//        std::vector<unsigned int>& blocks, unsigned int width);
//private:
//    BlockDef& getBlockAt(const glm::ivec3 &pos);
//    void addFaces(const glm::vec3 &offset, const std::vector<MeshPart> &meshParts, const glm::vec3& tint);
//
//    LocalDefinitionAtlas& defs;
//    LocalBiomeAtlas& biomes;
//
//    unsigned int indOffset = 0;
//    FarMapMeshDetails* meshDetails;
//
//    unsigned int width;
//    std::vector<unsigned int>& blocks;
//};