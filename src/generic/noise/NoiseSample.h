//
// Created by aurailus on 15/02/19.
//

#ifndef ZEUS_NOISESAMPLE_H
#define ZEUS_NOISESAMPLE_H

#include <vector>
#include <vec3.hpp>
#include <iostream>

class NoiseSample {
public:
    NoiseSample(int hPrecision, int vPrecision);

    void  set(glm::vec3 pos, float value);
    float get(glm::vec3 pos);

private:
    std::vector<std::vector<std::vector<float>>> data;

    int hPrecision;
    int vPrecision;
};


#endif //ZEUS_NOISESAMPLE_H
