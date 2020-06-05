//
// Created by aurailus on 2020-04-05.
//

#include "FarMapGen.h"

FarMapGen::FarMapGen(unsigned int seed, DefinitionAtlas& defs, BiomeAtlas& biomes, std::shared_ptr<MapGenProps> props) :
    seed(seed),
    defs(defs),
    props(props),
    biomes(biomes) {}

MeshFarMap FarMapGen::generateFarMap(glm::ivec3 mbPos, unsigned short downScale) {
    auto job = new FarMapJob();
    job->downScale = downScale;

    buildDensityMap(job, mbPos);

    delete job;
}

void FarMapGen::buildDensityMap(FarMapJob *job, glm::ivec3 mbPos) {
    job->temperature = {}; job->humidity = {}; job->roughness = {};

    job->temperature.fill([&](glm::ivec3 pos) {
        return props->temperature.GetValue(mbPos.x * 4 + pos.x / 4.f, 0, mbPos.z * 4 + pos.z / 4.f); }, 4);
    job->humidity.fill([&](glm::ivec3 pos) {
        return props->humidity.GetValue(mbPos.x * 4 + pos.x / 4.f, 0, mbPos.z * 4 + pos.z / 4.f); }, 4);
    job->roughness.fill([&](glm::ivec3 pos) {
        return props->roughness.GetValue(mbPos.x * 4 + pos.x / 4.f, 0, mbPos.z * 4 + pos.z / 4.f); }, 4);

    NoiseSample volume = {}, heightmap = {};

    volume.fill([&](glm::ivec3 pos) {
        auto& biome = biomes.getBiomeAt(job->temperature.get(pos), job->humidity.get(pos), job->roughness.get(pos));
        return biome.volume[biome.volume.size() - 1]->GetValue(mbPos.x * 4 + pos.x / 4.f, mbPos.y * 4 + pos.y / 4.f, mbPos.z * 4 + pos.z / 4.f);
    }, {4, 4});

    heightmap.fill([&](glm::ivec3 pos) {
        auto& biome = biomes.getBiomeAt(job->temperature.get(pos), job->humidity.get(pos), job->roughness.get(pos));
        return biome.heightmap[biome.heightmap.size() - 1]->GetValue(mbPos.x * 4 + pos.x / 4.f, 0, mbPos.z * 4 + pos.z / 4.f);
    }, 4);

    int sampleWid = 64 / job->downScale;
    float factor = (job->downScale == 2 ? 0.5f : job->downScale == 4 ? 1 : 2);

    for (int m = 0; m < pow(sampleWid, 3); m++) {
        glm::ivec3 lp = Space::Block::fromIndex(m);
        job->density[m] = (volume.get(glm::vec3(lp) * factor) + heightmap.get(glm::vec3(lp) * factor)) - (static_cast<float>(lp.y) * factor + mbPos.y * 64);
    }
}


