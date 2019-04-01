//
// Created by aurailus on 28/01/19.
//

#include "MapGen.h"
#include "../../client/engine/Timer.h"

MapGen::MapGen(unsigned int seed) : sampler(seed) {

    p_feature = NoiseParams {
            .PERLIN_TYPE = NoiseParams::PERLIN_3D,
            .NOISE_H_FACTOR = 60,
            .NOISE_V_FACTOR = 100,
            .SAMPLE_H_PRECISION = 4,
            .SAMPLE_V_PRECISION = 4,
            .NOISE_MULTIPLIER = 2
    };

    p_feature_scale = NoiseParams {
            .PERLIN_TYPE = NoiseParams::PERLIN_2D,
            .NOISE_H_FACTOR = 100,
            .NOISE_V_FACTOR = 0,
            .SAMPLE_H_PRECISION = 1,
            .SAMPLE_V_PRECISION = 1,
            .NOISE_MULTIPLIER = 1
    };

    p_density = NoiseParams {
        .PERLIN_TYPE = NoiseParams::PERLIN_2D,
        .NOISE_H_FACTOR = 2000,
        .NOISE_V_FACTOR = 0,
        .SAMPLE_H_PRECISION = 1,
        .SAMPLE_V_PRECISION = 1,
        .NOISE_MULTIPLIER = 100
    };

    p_density_variation = NoiseParams {
            .PERLIN_TYPE = NoiseParams::PERLIN_2D,
            .NOISE_H_FACTOR = 300,
            .NOISE_V_FACTOR = 0,
            .SAMPLE_H_PRECISION = 1,
            .SAMPLE_V_PRECISION = 1,
            .NOISE_MULTIPLIER = 50
    };

    p_density_variation_smaller = NoiseParams {
            .PERLIN_TYPE = NoiseParams::PERLIN_2D,
            .NOISE_H_FACTOR = 100,
            .NOISE_V_FACTOR = 0,
            .SAMPLE_H_PRECISION = 2,
            .SAMPLE_V_PRECISION = 2,
            .NOISE_MULTIPLIER = 25
    };

    p_flora_feature = NoiseParams {
            .PERLIN_TYPE = NoiseParams::PERLIN_2D,
            .NOISE_H_FACTOR = 100,
            .NOISE_V_FACTOR = 0,
            .SAMPLE_H_PRECISION = 2,
            .SAMPLE_V_PRECISION = 2,
            .NOISE_MULTIPLIER = 1
    };

    p_flora_smaller = NoiseParams {
            .PERLIN_TYPE = NoiseParams::PERLIN_2D,
            .NOISE_H_FACTOR = 25,
            .NOISE_V_FACTOR = 0,
            .SAMPLE_H_PRECISION = 4,
            .SAMPLE_V_PRECISION = 4,
            .NOISE_MULTIPLIER = 1
    };
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
    auto density_sample = sampler.sample(job.pos, p_density);
    auto density_variation_sample = sampler.sample(job.pos, p_density_variation);
    auto density_variation_smaller_sample = sampler.sample(job.pos, p_density_variation_smaller);
    auto feature_sample = sampler.sample(job.pos, p_feature);
    auto feature_scale_sample = sampler.sample(job.pos, p_feature_scale);

    glm::vec3 lp;

    for (int m = 0; m < 4096; m++) {
        ArrayTrans3D::indAssignVec(m, lp);

        job.density[m] = density_sample.get(lp)
                       + density_variation_sample.get(lp)
                       + density_variation_smaller_sample.get(lp)
                       + ((float)pow(feature_sample.get(lp) + 0.5, 2.0) - 0.5f) * 15
                       - ((job.pos.y * 16 + lp.y));
    }
}

void MapGen::fillChunk(MapGenJob &job) {
    auto flora_sample = sampler.sample(job.pos, p_flora_feature);
    auto flora_smaller_sample = sampler.sample(job.pos, p_flora_smaller);

    glm::vec3 lp;

    for (int m = 0; m < 4096; m++) {
        ArrayTrans3D::indAssignVec(m, lp);
        int d = job.depth[m];

        double grassType = (flora_sample.get(lp) + 0.5) * (flora_smaller_sample.get(lp) + 0.5);

        int tallGrass = (int)floor(grassType * 5.0 - 1);
        if (tallGrass > 0) tallGrass += 5;
        else tallGrass = 0;

        job.blocks[m] = d == 0 ? 0
                      : d == 1 ? tallGrass
                      : d == 2 ? 1
                      : d <= 5 ? 2
                      : 3;
    }
}
