#include <cmath>

//
// Created by aurailus on 28/01/19.
//

#include "MapGen.h"
#include "../../client/engine/Timer.h"
#include "../noise/NoiseSample.h"

MapGen::MapGen(unsigned int seed) {
    this->seed = seed;

    terrainFlatBase.SetFrequency(0.15);
    terrainFlatBase.SetPersistence(0.4);

    terrainFlat.SetSourceModule(0, terrainFlatBase);
    terrainFlat.SetScale(0.125);
    terrainFlat.SetBias(-0.75);

    terrainType.SetFrequency(0.05);
    terrainType.SetPersistence(0.25);

    terrainMountains.SetFrequency(0.1);

    terrainFinal.SetSourceModule(0, terrainFlat);
    terrainFinal.SetSourceModule(1, terrainMountains);

    terrainFinal.SetControlModule(terrainType);
    terrainFinal.SetBounds(0.0, 1000.0);
    terrainFinal.SetEdgeFalloff(0.1);

    floraNoise.SetFrequency(2);
    floraNoise.SetOctaveCount(4);

    floraTurbulence.SetSourceModule(0, floraNoise);
    floraTurbulence.SetFrequency(4.0);
    floraTurbulence.SetPower(0.125);

    floraFinal.SetSourceModule(0, floraTurbulence);
    floraFinal.SetScale(3);
    floraFinal.SetBias(1);
}

BlockChunk* MapGen::generate(glm::vec3 pos) {
    Timer t("Chunk Gen");

    MapGenJob job(pos);

    getDensityMap(job);
    getElevation(job);

    fillChunk(job);

    t.printElapsedMs();

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
    auto flora_2d_sample = NoiseSample::getSample(&floraFinal, job.pos, 16, 1, true);

    glm::vec3 lp;

    for (int m = 0; m < 4096; m++) {
        ArrayTrans3D::indAssignVec(m, lp);
        int d = job.depth[m];

        int grassType = min((int)std::floor(flora_2d_sample.get(lp)), 5);

        if (grassType > 0) grassType += 5;
        else grassType = 0;

        job.blocks[m] = d == 0 ? 0
                      : d == 1 ? grassType
                      : d == 2 ? 1
                      : d <= 3 ? 2
                      : 3;
    }
}
