//
// Created by aurailus on 15/02/19.
//
#include "NoiseSample.h"

NoiseSample::NoiseSample(glm::ivec2 precision) :
    prec(precision) {
    reserveSpace();
}

NoiseSample::NoiseSample(noise::module::Module &module, glm::vec3 chunkPos, glm::ivec2 precision, bool flat) :
    prec(precision) {
    reserveSpace();

    float offsetH = 16.f / prec.x;
    float offsetV = 16.f / prec.y;

    for (int i = 0; i <= prec.x; i++) {
        for (int j = 0; j <= prec.y; j++) {
            for (int k = 0; k <= prec.x; k++) {
                glm::vec3 pos = {(chunkPos.x * 16 + offsetH * i) / 16.f,
                                 flat ? 0 : (chunkPos.y * 16 + offsetV * j) / 16.f,
                                 (chunkPos.z * 16 + offsetH * k) / 16.f};
                set({i, j, k}, static_cast<float>(module.GetValue(pos.x, pos.y, pos.z)));
            }
        }
    }
}

void NoiseSample::set(glm::ivec3 pos, float value) {
    data[pos.x][pos.y][pos.z] = value;
}

float NoiseSample::get(const glm::ivec3& pos) {
    glm::vec3 prec3 {prec.x, prec.y, prec.x};

    const glm::ivec3 base = pos / (glm::ivec3(16) / glm::ivec3(prec3));
    const glm::vec3 factor = glm::floor(glm::mod(glm::vec3(pos), (glm::vec3(16.f) / prec3))) / 16.f * prec3;

    const auto& x0y0 = data[base.x][base.y];
    const auto& x1y0 = data[base.x + 1][base.y];

    //No Vertical Interpolation
    if (prec.y <= 1)
        return Interp::bilerp(x0y0[base.z], x1y0[base.z], x0y0[base.z + 1], x1y0[base.z + 1], factor.x, factor.z);

    const auto& x0y1 = data[base.x][base.y + 1];
    const auto& x1y1 = data[base.x + 1][base.y + 1];

    return Interp::trilerp(x0y0[base.z], x1y0[base.z], x0y0[base.z + 1], x1y0[base.z + 1],
            x0y1[base.z], x1y1[base.z], x0y1[base.z + 1], x1y1[base.z + 1], factor.x, factor.z, factor.y);
}

void NoiseSample::reserveSpace() {
    data.reserve(prec.x + 1);
    for (unsigned int i = 0; i <= prec.x; i++) {
        std::vector<std::vector<float>> subdata;
        subdata.reserve(prec.y + 1);
        for (int j = 0; j <= prec.y; j++)
            subdata.emplace_back(prec.x + 1);
        data.push_back(subdata);
    }
}
