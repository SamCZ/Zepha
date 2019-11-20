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

BlockChunk* MapGen::generate(glm::vec3 pos) {
    MapGenJob job(pos);

    getDensityMap(job);
    getElevation(job);
    fillChunk(job);

//    addTrees(job);

    return new BlockChunk(job.blocks, job.biomes, pos);
}

void MapGen::getElevation(MapGenJob &job) {

    //TODO: When generating mapblocks @ a time, don't do this unless necessary. Refer to already generated chunks. uwu
    MapGenJob* otherJob = nullptr;

    for (int i = 0; i < 256; i++) {
        int x = i % 16;
        int z = i / 16;

        int knownDepth = 16;

        if (job.density[VecUtils::vecToInd(x, 15, z)] > 0) {
            if (otherJob == nullptr) {
                otherJob = new MapGenJob(glm::vec3(job.pos.x, job.pos.y + 1, job.pos.z));
                getDensityMap(*otherJob);
            }

            for (int j = 0; j < 16; j++) {
                int otherInd = VecUtils::vecToInd(x, j, z);

                if (otherJob->density[otherInd] <= 0) {
                    knownDepth = j;
                    break;
                }
            }
        }
        else knownDepth = 0;

        for (int y = 15; y >= 0; y--) {
            int ind = VecUtils::vecToInd(x, y, z);

            if (job.density[ind] > 0) {
                knownDepth = min(knownDepth + 1, 16);
            }
            else knownDepth = 0;
            job.depth[ind] = knownDepth + (job.density[ind] - (int)job.density[ind]);
        }
    }

    delete otherJob;
}

void MapGen::getDensityMap(MapGenJob &job) {
    auto terrain_2d_sample = NoiseSample::getSample(&terrainFinal, job.pos, 4, 1, true);

    glm::vec3 lp;

    for (int m = 0; m < 4096; m++) {
        VecUtils::indAssignVec(m, lp);
        job.density[m] = terrain_2d_sample.get(lp) - (lp.y + job.pos.y * TransPos::CHUNK_SIZE);
    }
}

void MapGen::fillChunk(MapGenJob &job) {
//    auto grass_sample           = NoiseSample::getSample(&grassFinal,   job.pos, 8, 0, true);
//    auto flora_type_sample      = NoiseSample::getSample(&floraFinal,   job.pos, 2, 0, true);
//    auto flora_density_sample   = NoiseSample::getSample(&floraDensity, job.pos, 8, 0, true);

    auto temperature_sample = NoiseSample::getSample(&temperature, job.pos, 4, 4, false);
    auto humidity_sample    = NoiseSample::getSample(&humidity,    job.pos, 4, 4, false);
    auto roughness_sample   = NoiseSample::getSample(&roughness,   job.pos, 4, 4, false);

    glm::vec3 lp;

    for (int m = 0; m < 4096; m++) {
        VecUtils::indAssignVec(m, lp);

        auto biome = biomes.getBiomeAt(temperature_sample.get(lp), humidity_sample.get(lp), roughness_sample.get(lp));
        job.biomes[m] = biome.index;

        int d = std::floor(job.depth[m]);
//        int flora = DefinitionAtlas::AIR;
//
//        if (flora_density_sample.get(lp) >= 1) {
//            flora = FLOWERS[max(min((int) std::floor(flora_type_sample.get(lp)), 7), 0)];
//        }
//        else {
//            int grassType = min((int) std::floor(grass_sample.get(lp)), 5);
//            if (grassType > 0) flora = TALLGRASSES[grassType];
//        }

        job.blocks[m] = d <= 1 ? DefinitionAtlas::AIR
//                      : d <= 1 ? flora
                      : d <= 2 ? biome.topBlock
                      : d <= 4 ? biome.soilBlock
                               : biome.rockBlock;
    }
}

//void MapGen::addTrees(MapGenJob &job) {
//    auto flora_density_sample = NoiseSample::getSample(&floraDensity, job.pos, 8, 0, true);
//
//    glm::vec3 lp;
//
//    for (int m = 0; m < (int)pow(TransPos::CHUNK_SIZE, 3); m++) {
//        VecUtils::indAssignVec(m, lp);
//        int d = job.depth[m];
//
//        if (d == 1 && flora_density_sample.get(lp) <= -1) {
//            glm::vec3 p = lp;
//
//            addBlock(p, PLANT_STEM_BLOCK, job);
//            addBlock(p + glm::vec3{ 1, 0, 0}, LEAVES_BLOCK, job);
//            addBlock(p + glm::vec3{-1, 0, 0}, LEAVES_BLOCK, job);
//            addBlock(p + glm::vec3{ 0, 0, 1}, LEAVES_BLOCK, job);
//            addBlock(p + glm::vec3{ 0, 0,-1}, LEAVES_BLOCK, job);
//            addBlock(p + glm::vec3{ 0, 1, 0}, LEAVES_BLOCK, job);
//        }
//    }
//}

//void MapGen::addBlock(glm::vec3 lp, unsigned int block, MapGenJob &j) {
//    if (lp.x >= 0 && lp.x < TransPos::CHUNK_SIZE && lp.y >= 0 && lp.y < TransPos::CHUNK_SIZE && lp.z >= 0 && lp.z < TransPos::CHUNK_SIZE) {
//        j.blocks[VecUtils::vecToInd(lp)] = block;
//    }
//}