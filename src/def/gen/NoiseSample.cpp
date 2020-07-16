//
// Created by aurailus on 15/02/19.
//

#include "NoiseSample.h"

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
