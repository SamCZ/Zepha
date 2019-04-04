//
// Created by aurailus on 15/02/19.
//

#ifndef ZEUS_NOISESAMPLE_H
#define ZEUS_NOISESAMPLE_H

#include <vector>
#include <vec3.hpp>
#include <iostream>
#include <noise/noise.h>

class NoiseSample {
public:
    static NoiseSample getSample(noise::module::Module *module, glm::vec3 chunkPos, int hPrecision = 8, int vPrecision = 8, bool flat = false);

    NoiseSample(int hPrecision, int vPrecision);

    void  set(glm::vec3 pos, float value);
    float get(glm::vec3& pos);

private:
    std::vector<std::vector<std::vector<float>>> data;

    int hPrecision;
    int vPrecision;
};


#endif //ZEUS_NOISESAMPLE_H
