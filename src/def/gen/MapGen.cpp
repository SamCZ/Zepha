#include <cmath>

//
// Created by aurailus on 28/01/19.
//

#include "MapGen.h"
#include "../../util/Timer.h"
#include "NoiseSample.h"

MapGen::MapGen(unsigned int seed, DefinitionAtlas& atlas, BiomeAtlas& biomes) :
        seed(seed),
        atlas(atlas),
        biomes(biomes) {

    temperatureBase.SetSeed(seed);
    temperatureBase.SetFrequency(0.02);
    temperatureBase.SetOctaveCount(4);
    temperature.SetSourceModule(0, temperatureBase);
    temperature.SetScale(0.35);
    temperature.SetBias(0.25);

    humidityBase.SetSeed(seed + 5);
    humidityBase.SetFrequency(0.02);
    humidityBase.SetOctaveCount(4);
    humidity.SetSourceModule(0, humidityBase);
    humidity.SetScale(0.5);
    humidity.SetBias(0.5);

    roughnessBase.SetSeed(seed + 10);
    roughnessBase.SetFrequency(0.02);
    roughnessBase.SetOctaveCount(4);
    roughness.SetSourceModule(0, roughnessBase);
    roughness.SetScale(0.5);
    roughness.SetBias(0.5);




    //First stage smooth elevation
    worldElevationBase.SetSeed(seed);
    worldElevationBase.SetFrequency(0.002);
    worldElevationBase.SetOctaveCount(8);

    worldElevationScaled.SetSourceModule(0, worldElevationBase);
    worldElevationScaled.SetScale(250);
    worldElevationScaled.SetBias(32);

    //Smooth elevation features
    worldFeatureBase.SetSeed(seed);
    worldFeatureBase.SetFrequency(0.2);
    worldFeatureBase.SetOctaveCount(3);
    worldFeatureScaled.SetSourceModule(0, worldFeatureBase);
    worldFeatureScaled.SetScale(6);
    worldFeatureScaled.SetBias(6);

    //Smooth elevation combined
    worldSmoothElevation.SetSourceModule(0, worldElevationScaled);
    worldSmoothElevation.SetSourceModule(1, worldFeatureScaled);

    //Smooth mountain terrain
    mountainSmoothBase.SetSeed(seed);
    mountainSmoothBase.SetFrequency(0.05);
    mountainSmoothScaled.SetSourceModule(0, mountainSmoothBase);
    mountainSmoothScaled.SetScale(300);
    mountainSmoothScaled.SetBias(400);

    //Craggy mountains hold
    mountainRoughHoldBase.SetSeed(seed);
    mountainRoughHoldBase.SetFrequency(0.05);
    mountainRoughHoldScaled.SetSourceModule(0, mountainRoughHoldBase);
    mountainRoughHoldScaled.SetScale(0.5);
    mountainRoughHoldScaled.SetBias(0.5);

    //Craggy mountains
    mountainRoughBase.SetSeed(seed);
    mountainRoughBase.SetFrequency(0.75);
    mountainRoughBase.SetLacunarity(1.5);
    mountainRoughBase.SetOctaveCount(8);
    mountainRoughScaled.SetSourceModule(0, mountainRoughBase);
    mountainRoughScaled.SetScale(20);
    mountainRoughScaled.SetBias(20);

    mountainRoughMultiplied.SetSourceModule(0, mountainRoughHoldScaled);
    mountainRoughMultiplied.SetSourceModule(1, mountainRoughScaled);

    mountainNoise.SetSourceModule(0, mountainSmoothScaled);
    mountainNoise.SetSourceModule(1, mountainRoughMultiplied);

    //Noise for the strength "multiplier" for mountains
    mountainMultiplierBase.SetSeed(seed);
    mountainMultiplierBase.SetFrequency(0.02);
    mountainMultiplierBase.SetPersistence(0.25f);
    mountainMultiplierScaled.SetSourceModule(0, mountainMultiplierBase);
//    mountainMultiplierScaled.SetScale(2);
//    mountainMultiplierScaled.SetBias(-1);
    mountainMultiplierClamped.SetSourceModule(0, mountainMultiplierScaled);
    mountainMultiplierClamped.SetBounds(0, 1);

    //Multiply mountain terrain by the multiplier
    mountainMultiplied.SetSourceModule(0, mountainMultiplierClamped);
    mountainMultiplied.SetSourceModule(1, mountainNoise);

    //Add both the general elevation and the mountain terrain
    terrainFinal.SetSourceModule(0, worldSmoothElevation);
    terrainFinal.SetSourceModule(1, mountainMultiplied);

    grassNoise.SetFrequency(2);
    grassNoise.SetOctaveCount(3);

    grassTurbulence.SetSourceModule(0, grassNoise);
    grassTurbulence.SetFrequency(4.0);
    grassTurbulence.SetPower(0.125);

    grassFinal.SetSourceModule(0, grassTurbulence);
    grassFinal.SetScale(3);
    grassFinal.SetBias(1);

    floraNoise.SetFrequency(2);
    floraNoise.SetOctaveCount(3);

    floraTurbulence.SetSourceModule(0, floraNoise);
    floraTurbulence.SetFrequency(4.0);
    floraTurbulence.SetPower(0.125);

    floraFinal.SetSourceModule(0, floraTurbulence);
    floraFinal.SetScale(4);
    floraFinal.SetBias(4);

    floraDensity.SetFrequency(4);

    biomeTemp.SetFrequency(0.1);
    biomeTemp.SetOctaveCount(12);
    biomeTemp.SetPersistence(0.40);
}

std::vector<BlockChunk*> MapGen::generateMapBlock(glm::vec3 mbPos) {
    std::array<std::pair<MapGenJob, BlockChunk*>, 64> chunks {};

    // Go top down
    for (short i = 3; i >= 0; i--) {
        for (short j = 0; j < 4; j++) {
            for (short k = 0; k < 4; k++) {
                glm::vec3 pos {j, i, k};
                generateChunk(chunks, pos, pos + mbPos * 4);
            }
        }
    }

    std::vector<BlockChunk*> returns {};
    returns.reserve(64);
    for (auto& pair : chunks) {
        returns.push_back(pair.second);
    }

    return returns;
}

void MapGen::generateChunk(std::array<std::pair<MapGenJob, BlockChunk*>, 64>& chunks, glm::vec3 localPos, glm::vec3 worldPos) {
    unsigned short index = localPos.x + 4 * (localPos.y + 4 * localPos.z);
    auto& chunk = chunks[index];
    chunk.second = new BlockChunk();
    chunk.second->pos = worldPos;

    buildDensityMap(chunk.first, worldPos);
    buildElevationMap(chunks, chunk, localPos, worldPos);

    populateChunk(chunk, worldPos);
}

void MapGen::buildDensityMap(MapGenJob &job, const glm::vec3& worldPos) {
    auto sTerrainFinal = NoiseSample(terrainFinal, worldPos, {4, 1}, true);

    glm::vec3 lp;
    for (int m = 0; m < 4096; m++) {
        VecUtils::indAssignVec(m, lp);
        job.density[m] = sTerrainFinal.get(lp) - (lp.y + worldPos.y * 16);
    }
}

void MapGen::buildElevationMap(std::array<std::pair<MapGenJob, BlockChunk *>, 64>& chunks,
                               pair<MapGenJob, BlockChunk *> &chunk, const glm::vec3 &localPos, const glm::vec3 &worldPos) {

    MapGenJob* upperChunk = nullptr;

    for (int i = 0; i < 256; i++) {
        const int x = i % 16;
        const int z = i / 16;

        short depth = 16;

        if (chunk.first.density[VecUtils::vecToInd(x, 15, z)] > 0) {
            if (localPos.y < 3) {
                unsigned short index = localPos.x + 4 * (localPos.y + 1 + 4 * localPos.z);
                upperChunk = &chunks[index].first;
            }
            if (upperChunk == nullptr) {
                upperChunk = new MapGenJob();
                buildDensityMap(*upperChunk, worldPos + glm::vec3 {0, 1, 0});
            }

            for (int j = 0; j < 16; j++) {
                int ind = VecUtils::vecToInd(x, j, z);

                if (upperChunk->density[ind] <= 0) {
                    depth = j;
                    break;
                }
            }
        }
        else depth = 0;

        for (int y = 15; y >= 0; y--) {
            int ind = VecUtils::vecToInd(x, y, z);

            if (chunk.first.density[ind] > 0) {
                depth = std::min(depth + 1, 16);
            }
            else depth = 0;

            chunk.first.depth[ind] = depth + (chunk.first.density[ind] - static_cast<int>(chunk.first.density[ind]));
        }
    }

    if (localPos.y >= 3) delete upperChunk;
}

void MapGen::populateChunk(std::pair<MapGenJob, BlockChunk*>& chunk, const glm::vec3& worldPos) {
    auto sTemperature = NoiseSample(temperature, worldPos, {4, 4}, false);
    auto sHumidity    = NoiseSample(humidity,    worldPos, {4, 4}, false);
    auto sRoughness   = NoiseSample(roughness,   worldPos, {4, 4}, false);

    glm::vec3 lp;

    for (int m = 0; m < 4096; m++) {
        VecUtils::indAssignVec(m, lp);

        auto biome = biomes.getBiomeAt(sTemperature.get(lp), sHumidity.get(lp), sRoughness.get(lp));
        chunk.second->biomes[m] = biome.index;

        int d = std::floor(chunk.first.depth[m]);

        chunk.second->blocks[m] =
                d <= 1 ? DefinitionAtlas::AIR
              : d <= 2 ? biome.topBlock
              : d <= 4 ? biome.soilBlock
              : biome.rockBlock;
    }

    chunk.second->mgRegenEmpty();
}