//
// Created by aurailus on 15/02/19.
//

#include <glm/glm.hpp>

#include "NoiseSample.h"

#include "../../util/Interp.h"
#include "../../util/Util.h"

NoiseSample::NoiseSample(unsigned int precision, float scaleBy) : NoiseSample({precision, precision}, {scaleBy, scaleBy}) {}

NoiseSample::NoiseSample(glm::ivec2 precision, glm::vec2 scaleBy) :
    precision(precision.x, precision.y, precision.x),
    scaleBy(scaleBy.x, scaleBy.y, scaleBy.x) {
    // Reserve space in the vector.
    for (unsigned int i = 0; i < this->precision.y + 1; i++) {
        data.emplace_back();
        for (unsigned int j = 0; j < this->precision.x + 1; j++) {
            data[i].emplace_back();
            for (unsigned int k = 0; k < this->precision.z + 1; k++) {
                data[i][j].emplace_back();
            }
        }
    }
}

void NoiseSample::populate(const NoiseSample::fill_function &fn) {
    glm::vec3 pos;
    for (pos.x = 0; pos.x <= precision.x; pos.x++)
        for (pos.y = 0; pos.y <= precision.y; pos.y++)
            for (pos.z = 0; pos.z <= precision.z; pos.z++) {
                glm::vec3 queryPos = pos / glm::vec3(precision) * scaleBy;
                if (queryPos.y == NAN) queryPos.y = 0;
                data[pos.y][pos.x][pos.z] = fn(queryPos);
            }
}

float NoiseSample::get(glm::vec3 pos) {
    glm::vec3 scaled = pos * glm::vec3(precision) / scaleBy;

    glm::vec3 a = glm::floor(scaled);
    glm::vec3 factor = scaled - glm::floor(scaled);
    glm::vec3 b = {fmin(a.x + ceil(factor.x), precision.x), fmin(a.y + ceil(factor.y), precision.y), fmin(a.z + ceil(factor.z), precision.z)};

    assert(a.x + factor.x <= precision.x && a.y + factor.y <= precision.y && a.z + factor.z <= precision.z);

    const auto& p00 = data[a.y][a.x];
    const auto& p10 = data[a.y][b.x];

    // No vertical interpolation
    if (precision.y == 0) return Interp::bilerp(p00[a.z], p10[a.z], p00[b.z], p10[b.z], factor.x, factor.z);

    const auto& p01 = data[b.y][a.x];
    const auto& p11 = data[b.y][b.x];

    return Interp::trilerp(p00[a.z], p10[a.z], p00[b.z], p10[b.z],
        p01[a.z], p11[a.z], p01[b.z], p11[b.z], factor.x, factor.z, factor.y);
}
