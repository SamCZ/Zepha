//
// Created by aurailus on 28/01/19.
//

#include "MapGen.h"

MapGen::MapGen(unsigned int seed) : sampler(seed) {

    p_feature = NoiseParams {
            .PERLIN_TYPE = NoiseParams::PERLIN_3D,
            .NOISE_H_FACTOR = 30,
            .NOISE_V_FACTOR = 200,
            .SAMPLE_H_PRECISION = 4,
            .SAMPLE_V_PRECISION = 4,
            .NOISE_MULTIPLIER = 1
    };

    p_feature_scale = NoiseParams {
            .PERLIN_TYPE = NoiseParams::PERLIN_2D,
            .NOISE_H_FACTOR = 100,
            .NOISE_V_FACTOR = 0,
            .SAMPLE_H_PRECISION = 1,
            .SAMPLE_V_PRECISION = 1,
            .NOISE_MULTIPLIER = 1
    };

    p_elevation = NoiseParams {
        .PERLIN_TYPE = NoiseParams::PERLIN_2D,
        .NOISE_H_FACTOR = 2000,
        .NOISE_V_FACTOR = 0,
        .SAMPLE_H_PRECISION = 1,
        .SAMPLE_V_PRECISION = 1,
        .NOISE_MULTIPLIER = 200
    };

    p_elevation_variation = NoiseParams {
            .PERLIN_TYPE = NoiseParams::PERLIN_2D,
            .NOISE_H_FACTOR = 300,
            .NOISE_V_FACTOR = 0,
            .SAMPLE_H_PRECISION = 1,
            .SAMPLE_V_PRECISION = 1,
            .NOISE_MULTIPLIER = 100
    };

    p_elevation_variation_smaller = NoiseParams {
            .PERLIN_TYPE = NoiseParams::PERLIN_2D,
            .NOISE_H_FACTOR = 100,
            .NOISE_V_FACTOR = 0,
            .SAMPLE_H_PRECISION = 2,
            .SAMPLE_V_PRECISION = 2,
            .NOISE_MULTIPLIER = 50
    };
}

BlockChunk* MapGen::generate(glm::vec3 pos) {
    MapGenJob j(pos);

    buildElevation(j);
    fillChunk(j);

    return new BlockChunk(j.blocks);
}

void MapGen::buildElevation(MapGenJob &j) {
    auto elevation_sample = sampler.sample(j.pos, p_elevation);
    auto elevation_variation_sample = sampler.sample(j.pos, p_elevation_variation);
    auto elevation_variation_smaller_sample = sampler.sample(j.pos, p_elevation_variation_smaller);
    auto feature_sample = sampler.sample(j.pos, p_feature);
    auto feature_scale_sample = sampler.sample(j.pos, p_feature_scale);

    glm::vec3 lp;

    for (int m = 0; m < 4096; m++) {
        ArrayTrans3D::indAssignVec(m, lp);

        j.density[m] = elevation_sample.get(lp)
                     + elevation_variation_sample.get(lp)
                     + elevation_variation_smaller_sample.get(lp)
                     + ((float)pow(feature_sample.get(lp) + 0.5, 2.0) - 0.5f) * 50 * (feature_scale_sample.get(lp) + 0.5f)
                     - ((j.pos.y * 16 + lp.y));
    }
}

void MapGen::fillChunk(MapGenJob &j) {
    glm::vec3 lp;

    for (int m = 0; m < 4096; m++) {
        ArrayTrans3D::indAssignVec(m, lp);
        j.blocks[m] = j.density[m] > 0 ? 3 : 0;
    }
}
