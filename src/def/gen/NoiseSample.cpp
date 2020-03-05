//
// Created by aurailus on 15/02/19.
//
#include "NoiseSample.h"

void NoiseSample::fill(const NoiseSample::fill_function &fun, float precision) {
    fill(fun, {precision, 1});
}

void NoiseSample::fill(const NoiseSample::fill_function &fun, glm::ivec2 precision) {
    this->precision = precision;
    reserve();

    float offsetH = 16.f / precision.x;
    float offsetV = 16.f / precision.y;

    // Iterate over the array
    for (int i = 0; i <= precision.x; i++) {
        for (int j = 0; j <= (precision.y == 1 ? 0 : precision.y); j++) {
            for (int k = 0; k <= precision.x; k++) {
                set({i, j, k}, fun({ offsetH * i, offsetV * j, offsetH * k }));
            }
        }
    }
}

float NoiseSample::get(glm::ivec3 localPos) {
    assert(precision.x != 0);
    assert(precision.y != 0);

    glm::vec3 prec3 {precision.x, precision.y, precision.x};

    if (localPos.x == 16) localPos.x = 15;
    if (localPos.y == 16) localPos.y = 15;
    if (localPos.z == 16) localPos.z = 15;
    glm::ivec3 base = localPos / (glm::ivec3(16) / glm::ivec3(prec3));

    const glm::vec3 factor = glm::floor(glm::mod(glm::vec3(localPos), (glm::vec3(16.f) / prec3))) / 16.f * prec3;

    const auto& x0y0 = data[base.x][base.y];
    const auto& x1y0 = data[base.x + 1][base.y];

    //No Vertical Interpolation
    if (precision.y <= 1) return Interp::bilerp(x0y0[base.z], x1y0[base.z], x0y0[base.z + 1], x1y0[base.z + 1], factor.x, factor.z);

    const auto& x0y1 = data[base.x][base.y + 1];
    const auto& x1y1 = data[base.x + 1][base.y + 1];

    return Interp::trilerp(
        x0y0[base.z], x1y0[base.z], x0y0[base.z + 1], x1y0[base.z + 1],
        x0y1[base.z], x1y1[base.z], x0y1[base.z + 1], x1y1[base.z + 1], factor.x, factor.z, factor.y);
}

void NoiseSample::set(glm::ivec3 localPos, float value) {
    data[localPos.x][localPos.y][localPos.z] = value;
}

void NoiseSample::reserve() {
    data.reserve(precision.x + 1);
    for (unsigned int i = 0; i <= precision.x; i++) {
        std::vector<std::vector<float>> subdata;
        subdata.reserve(precision.y + 1);
        for (int j = 0; j <= precision.y; j++)
            subdata.emplace_back(precision.x + 1);
        data.push_back(subdata);
    }
}