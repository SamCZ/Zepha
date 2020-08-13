//
// Created by aurailus on 15/02/19.
//

#include "NoiseSample.h"

NoiseSample::NoiseSample(unsigned int precision, float scaleBy) : NoiseSample({precision, precision}, {scaleBy, scaleBy}) {}

NoiseSample::NoiseSample(glm::ivec2 precision, glm::vec2 scaleBy) :
    precision(precision.x, precision.y, precision.x),
    scaleBy(scaleBy.x, scaleBy.y, scaleBy.x) {
    data.resize((this->precision.x + 1) * (this->precision.y + 1) * (this->precision.z + 1));
}

void NoiseSample::populate(const NoiseSample::fill_function &fn) {
    glm::vec3 pos;
    for (pos.x = 0; pos.x <= precision.x; pos.x++)
        for (pos.y = 0; pos.y <= precision.y; pos.y++)
            for (pos.z = 0; pos.z <= precision.z; pos.z++) {
                glm::vec3 queryPos = pos / glm::vec3(precision) * scaleBy;
                if (queryPos.y == NAN) queryPos.y = 0;
                data[index(pos.x, pos.y, pos.z)] = fn(queryPos);
            }
}
