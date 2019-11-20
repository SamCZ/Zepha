//
// Created by aurailus on 15/02/19.
//

#include <cmath>
#include <glm/vec4.hpp>
#include "NoiseSample.h"
#include "../../util/Interp.h"
#include "../../util/TransPos.h"

NoiseSample::NoiseSample(int hPrecision, int vPrecision) :
    hPrecision(hPrecision),
    vPrecision(vPrecision),
    data(std::vector<std::vector<std::vector<float>>> {}) {

    data.reserve((unsigned int)hPrecision + 1);

    for (int i = 0; i <= hPrecision; i++) {
        std::vector<std::vector<float>> subdata;
        subdata.reserve((unsigned int)vPrecision + 1);

        for (int j = 0; j <= vPrecision; j++) {
            subdata.emplace_back((unsigned int)hPrecision + 1);
        }

        data.push_back(subdata);
    }
}

void NoiseSample::set(glm::vec3 pos, float value) {
    data[(int)pos.x][(int)pos.y][(int)pos.z] = value;
}

float NoiseSample::get(const glm::vec3& pos) {
    const glm::ivec3 iPos {pos};

    const glm::ivec3 base {
        iPos.x / (16 / hPrecision),
        iPos.y / (16 / vPrecision),
        iPos.z / (16 / hPrecision) };

    const glm::vec3 factor {
        iPos.x % (16 / hPrecision) / 16.f * hPrecision,
        iPos.y % (16 / vPrecision) / 16.f * vPrecision,
        iPos.z % (16 / hPrecision) / 16.f * hPrecision };

    const auto& x0y0 = data[base.x][base.y];
    const auto& x1y0 = data[base.x + 1][base.y];

    //No Vertical Interpolation
    if (vPrecision <= 1)
        return Interp::bilerp(x0y0[base.z], x1y0[base.z], x0y0[base.z + 1], x1y0[base.z + 1], factor.x, factor.z);

    const auto& x0y1 = data[base.x][base.y + 1];
    const auto& x1y1 = data[base.x + 1][base.y + 1];

    return Interp::trilerp(x0y0[base.z], x1y0[base.z], x0y0[base.z + 1], x1y0[base.z + 1],
            x0y1[base.z], x1y1[base.z], x0y1[base.z + 1], x1y1[base.z + 1], factor.x, factor.z, factor.y);
}

NoiseSample NoiseSample::getSample(noise::module::Module *module, glm::vec3 chunkPos, int hPrecision, int vPrecision, bool flat) {
    NoiseSample s(hPrecision, vPrecision);

    float offsetH = 16.f / hPrecision;
    float offsetV = 16.f / vPrecision;

    for (int i = 0; i <= hPrecision; i++) {
        float xPos = (chunkPos.x * 16 + offsetH * i) / 16.f;
        for (int j = 0; j <= vPrecision; j++) {
            float yPos = flat ? 0 : (chunkPos.y * 16 + offsetV * j) / 16.f;
            for (int k = 0; k <= hPrecision; k++) {
                float zPos = (chunkPos.z * 16 + offsetH * k) / 16.f;
                s.set({i, j, k}, static_cast<float>(module->GetValue(xPos, yPos, zPos)));
            }
        }
    }

    return std::move(s);
}
