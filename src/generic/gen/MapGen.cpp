#include <cmath>

//
// Created by aurailus on 28/01/19.
//

#include "MapGen.h"
#include "../../client/engine/Timer.h"
#include "NoiseSample.h"

MapGen::MapGen(unsigned int seed) {
    this->seed = seed;

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

    terrainFinal.SetSourceModule(0, terrainFlat);
    terrainFinal.SetSourceModule(1, terrainMountains);

    terrainFinal.SetControlModule(terrainType);
    terrainFinal.SetBounds(0.0, 1000.0);
    terrainFinal.SetEdgeFalloff(0.1);

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
    floraFinal.SetScale(3.5);
    floraFinal.SetBias(3.5);

    floraDensity.SetFrequency(4);
}

BlockChunk* MapGen::generate(glm::vec3 pos) {
    MapGenJob job(pos);

    getDensityMap(job);
    getElevation(job);

    fillChunk(job);

    return new BlockChunk(job.blocks, pos);
}

void MapGen::getElevation(MapGenJob &job) {

    MapGenJob* otherJob = nullptr;

    for (int i = 0; i < 256; i++) {
        int x = i % 16;
        int z = i / 16;

        int knownDepth = 16;

        if (job.density[ArrayTrans3D::vecToInd(x, 15, z)] > 0) {
            if (otherJob == nullptr) {
                otherJob = new MapGenJob(glm::vec3(job.pos.x, job.pos.y + 1, job.pos.z));
                getDensityMap(*otherJob);
            }

            for (int j = 0; j < 16; j++) {
                int otherInd = ArrayTrans3D::vecToInd(x, j, z);

                if (otherJob->density[otherInd] <= 0) {
                    knownDepth = j;
                    break;
                }
            }
        }
        else knownDepth = 0;

        for (int y = 15; y >= 0; y--) {
            int ind = ArrayTrans3D::vecToInd(x, y, z);

            if (job.density[ind] > 0) {
                knownDepth = min(knownDepth + 1, 16);
            }
            else knownDepth = 0;

            job.depth[ind] = knownDepth;
        }
    }

    delete otherJob;
}

void MapGen::getDensityMap(MapGenJob &job) {
    auto terrain_2d_sample = NoiseSample::getSample(&terrainFinal, job.pos, 4, 1, true);

    glm::vec3 lp;

    for (int m = 0; m < 4096; m++) {
        ArrayTrans3D::indAssignVec(m, lp);
        job.density[m] = terrain_2d_sample.get(lp) * 24.0f - (lp.y + job.pos.y * 16);
    }
}

void MapGen::fillChunk(MapGenJob &job) {
    auto grass_sample = NoiseSample::getSample(&grassFinal, job.pos, 16, 0, true);
    auto flora_type_sample = NoiseSample::getSample(&floraFinal, job.pos, 4, 0, true);
    auto flora_density_sample = NoiseSample::getSample(&floraDensity, job.pos, 8, 0, true);

    glm::vec3 lp;

    for (int m = 0; m < 4096; m++) {
        ArrayTrans3D::indAssignVec(m, lp);
        int d = job.depth[m];

        int floraId = 0;
        bool flower = false;

        if (flora_density_sample.get(lp) > 1) flower = true;

        if (flower) {
            int flowerType = max(min((int)std::floor(flora_type_sample.get(lp)), 8), 0);
            floraId = flowerType + 10;
        }
        else {
            int grassType = min((int)std::floor(grass_sample.get(lp)), 5);
            if (grassType > 0) floraId = grassType + 5;
        }

        job.blocks[m] = d == 0 ? 0
                      : d == 1 ? floraId
                      : d == 2 ? 1
                      : d <= 3 ? 2
                      : 3;
    }
}
