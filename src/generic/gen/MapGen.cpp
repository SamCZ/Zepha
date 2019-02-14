//
// Created by aurailus on 28/01/19.
//

#include "MapGen.h"

void MapGen::init(unsigned int seed) {
}

float lerp(float p0, float p1, float factor) {
    return p0 + factor * (p1 - p0);
}

MapGen::noise_points MapGen::getNoisePoints(glm::vec3 pos, int hScale, int vScale) {
    MapGen::noise_points points;

    float offsetH = 16.0f / hScale;
    float offsetV = 16.0f / vScale;

    for (int i = 0; i <= hScale; i++) {
        std::vector<std::vector<float>> slice;
        for (int j = 0; j <= vScale; j++) {
            std::vector<float> row;
            for (int k = 0; k <= hScale; k++) {
                row.push_back(noiseAt(
                        pos.x * 16 + offsetH * i,
                        pos.y * 16 + offsetV * j,
                        pos.z * 16 + offsetH * k));
            }
            slice.push_back(row);
        }
        points.push_back(slice);
    }

    return std::move(points);
}

float MapGen::noiseAt(float x, float y, float z) {
    return (float)p.noise(x / BASE_H_FACTOR, y / BASE_V_FACTOR, z / BASE_H_FACTOR);
}

BlockChunk* MapGen::generate(glm::vec3 pos) {
    MapGenJob j(pos);

    MapGen::noise_points points = getNoisePoints(pos, BASE_H_PRECISION, BASE_V_PRECISION);

    glm::vec3 lp(0, 0, 0);

    float offsetH = 16.0f / BASE_H_PRECISION;
    float offsetV = 16.0f / BASE_V_PRECISION;

    for (int i = 0; i < 4096; i++) {
        ArrayTrans3D::indAssignVec(i, lp);

        auto xBase = (int)floor(lp.x / offsetH);
        auto yBase = (int)floor(lp.y / offsetV);
        auto zBase = (int)floor(lp.z / offsetH);

        auto p000 = points[xBase][yBase][zBase];
        auto p100 = points[xBase + 1][yBase][zBase];
        auto p001 = points[xBase][yBase][zBase + 1];
        auto p101 = points[xBase + 1][yBase][zBase + 1];

        auto p010 = points[xBase][yBase + 1][zBase];
        auto p110 = points[xBase + 1][yBase + 1][zBase];
        auto p011 = points[xBase][yBase + 1][zBase + 1];
        auto p111 = points[xBase + 1][yBase + 1][zBase + 1];

        float xFac = ((int)lp.x % (int)offsetH) / offsetH;
        float yFac = ((int)lp.y % (int)offsetV) / offsetV;
        float zFac = ((int)lp.z % (int)offsetH) / offsetH;

        float density =
                lerp(lerp(lerp(p000, p100, xFac), lerp(p001, p101, xFac), zFac),
                     lerp(lerp(p010, p110, xFac), lerp(p011, p111, xFac), zFac), yFac);

        float hills = (float)(p.noise((pos.x * 16 + lp.x) / HILLS_H_FACTOR, 0, (pos.x * 16 + lp.z) / HILLS_H_FACTOR) + 1) / 2.0f;
        hills *= HILLS_EFFECT_MULT;

        density -= ((pos.y * 16 + lp.y) / (HILLS_BASE_EFFECT * hills));

        j.blocks[i] = density > 0.5;
    }


    return new BlockChunk(j.blocks);
}