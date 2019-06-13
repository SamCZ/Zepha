#include <cmath>

//
// Created by aurailus on 28/01/19.
//

#include "MapGen.h"
#include "../../util/Timer.h"
#include "NoiseSample.h"
#include "../../util/Vec.h"

MapGen::MapGen(unsigned int seed, BlockAtlas& atlas) {
    this->seed = seed;

    //Get Block Indexes

    GRASS_BLOCK = atlas.fromIdentifier("default:grass").getIndex();
    DIRT_BLOCK  = atlas.fromIdentifier("default:dirt").getIndex();
    STONE_BLOCK = atlas.fromIdentifier("default:stone").getIndex();

    PLANT_STEM_BLOCK = atlas.fromIdentifier("default:wood").getIndex();
    LEAVES_BLOCK     = atlas.fromIdentifier("default:leaves").getIndex();

    for (int i = 1; i <= 5; i++) {
        TALLGRASSES[i] = atlas.fromIdentifier("default:tallgrass_" + to_string(i)).getIndex();
    }

    FLOWERS[0] = atlas.fromIdentifier("default:flower_dandelion_yellow").getIndex();
    FLOWERS[1] = atlas.fromIdentifier("default:flower_viola").getIndex();
    FLOWERS[2] = atlas.fromIdentifier("default:flower_geranium").getIndex();
    FLOWERS[3] = atlas.fromIdentifier("default:flower_tulip").getIndex();
    FLOWERS[4] = atlas.fromIdentifier("default:flower_dandelion_white").getIndex();
    FLOWERS[5] = atlas.fromIdentifier("default:flower_rose").getIndex();
    FLOWERS[6] = atlas.fromIdentifier("default:flower_mushroom_red").getIndex();
    FLOWERS[7] = atlas.fromIdentifier("default:flower_mushroom_brown").getIndex();

    //Set up Noise Parameters

    terrainGeneralElevation.SetFrequency(0.05);
    terrainGeneralElevation.SetPersistence(0.4);
    terrainGeneralElevation.SetOctaveCount(6);

    terrainFlatBase.SetFrequency(0.15);
    terrainFlatBase.SetPersistence(0.4);
    terrainFlatBase.SetOctaveCount(4);

    terrainFlat.SetSourceModule(0, terrainFlatBase);
    terrainFlat.SetScale(0.125);
    terrainFlat.SetBias(-0.75);

    terrainType.SetFrequency(0.05);
    terrainType.SetPersistence(0.25);
    terrainType.SetOctaveCount(4);

    terrainMountains.SetFrequency(0.1);
    terrainMountains.SetOctaveCount(4);

    terrainPreElevation.SetSourceModule(0, terrainFlat);
    terrainPreElevation.SetSourceModule(1, terrainMountains);

    terrainPreElevation.SetControlModule(terrainType);
    terrainPreElevation.SetBounds(0.0, 1000.0);
    terrainPreElevation.SetEdgeFalloff(0.1);

    terrainFinal.SetSourceModule(0, terrainPreElevation);
    terrainFinal.SetSourceModule(1, terrainGeneralElevation);

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
    addTrees(job);

    return new BlockChunk(job.blocks, pos);
}

void MapGen::getElevation(MapGenJob &job) {

    MapGenJob* otherJob = nullptr;

    for (int i = 0; i < pow(TransPos::CHUNK_SIZE, 2); i++) {
        int x = i % TransPos::CHUNK_SIZE;
        int z = i / TransPos::CHUNK_SIZE;

        int knownDepth = 16;

        if (job.density[VecUtils::vecToInd(x, (TransPos::CHUNK_SIZE - 1), z)] > 0) {
            if (otherJob == nullptr) {
                otherJob = new MapGenJob(glm::vec3(job.pos.x, job.pos.y + 1, job.pos.z));
                getDensityMap(*otherJob);
            }

            for (int j = 0; j < TransPos::CHUNK_SIZE; j++) {
                int otherInd = VecUtils::vecToInd(x, j, z);

                if (otherJob->density[otherInd] <= 0) {
                    knownDepth = j;
                    break;
                }
            }
        }
        else knownDepth = 0;

        for (int y = (TransPos::CHUNK_SIZE - 1); y >= 0; y--) {
            int ind = VecUtils::vecToInd(x, y, z);

            if (job.density[ind] > 0) {
                knownDepth = min(knownDepth + 1, 16);
            }
            else knownDepth = 0;

            job.depth[ind] = knownDepth;
            job.depthFloat[ind] = knownDepth + (job.density[ind] - (int)job.density[ind]);
        }
    }

    delete otherJob;
}

void MapGen::getDensityMap(MapGenJob &job) {
    auto terrain_2d_sample = NoiseSample::getSample(&terrainFinal, job.pos, 4, 1, true);

    glm::vec3 lp;

    for (int m = 0; m < (int)pow(TransPos::CHUNK_SIZE, 3); m++) {
        VecUtils::indAssignVec(m, lp);
        job.density[m] = terrain_2d_sample.get(lp) * 24.0f - (lp.y + job.pos.y * TransPos::CHUNK_SIZE);
    }
}

void MapGen::fillChunk(MapGenJob &job) {
    auto grass_sample = NoiseSample::getSample(&grassFinal, job.pos, 8, 0, true);
    auto flora_type_sample = NoiseSample::getSample(&floraFinal, job.pos, 2, 0, true);
    auto flora_density_sample = NoiseSample::getSample(&floraDensity, job.pos, 8, 0, true);

//    auto biome_sample = NoiseSample::getSample(&biomeTemp, job.pos, 1, 0, true);

    glm::vec3 lp;

    for (int m = 0; m < (int)pow(TransPos::CHUNK_SIZE, 3); m++) {
        VecUtils::indAssignVec(m, lp);

        int d = job.depth[m];
        int flora = AIR;

        if (flora_density_sample.get(lp) >= 1) {
            flora = FLOWERS[max(min((int) std::floor(flora_type_sample.get(lp)), 7), 0)];
        }
        else {
            int grassType = min((int) std::floor(grass_sample.get(lp)), 5);
            if (grassType > 0) flora = TALLGRASSES[grassType];
        }

        job.blocks[m] = d <= 0 ? AIR
                      : d <= 1 ? flora
                      : d <= 2 ? GRASS_BLOCK
                      : d <= 3 ? DIRT_BLOCK
                               : STONE_BLOCK;
    }
}

void MapGen::addTrees(MapGenJob &job) {
    auto flora_density_sample = NoiseSample::getSample(&floraDensity, job.pos, 8, 0, true);

    glm::vec3 lp;

    for (int m = 0; m < (int)pow(TransPos::CHUNK_SIZE, 3); m++) {
        VecUtils::indAssignVec(m, lp);
        int d = job.depth[m];

        if (d == 1 && flora_density_sample.get(lp) <= -1) {
            glm::vec3 p = lp;

            addBlock(p, PLANT_STEM_BLOCK, job);
            addBlock(p + glm::vec3( 1,  0,  0), LEAVES_BLOCK, job);
            addBlock(p + glm::vec3(-1,  0,  0), LEAVES_BLOCK, job);
            addBlock(p + glm::vec3( 0,  0,  1), LEAVES_BLOCK, job);
            addBlock(p + glm::vec3( 0,  0, -1), LEAVES_BLOCK, job);
            addBlock(p + glm::vec3( 0,  1,  0), LEAVES_BLOCK, job);
        }
    }
}

void MapGen::addBlock(glm::vec3 lp, int block, MapGenJob &j) {
    if (lp.x >= 0 && lp.x < TransPos::CHUNK_SIZE && lp.y >= 0 && lp.y < TransPos::CHUNK_SIZE && lp.z >= 0 && lp.z < TransPos::CHUNK_SIZE) {
        j.blocks[VecUtils::vecToInd(&lp)] = block;
    }
}