//
// Created by aurailus on 15/02/19.
//

#pragma once

#include <vector>
#include <functional>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class NoiseSample {
public:
    typedef std::function<float(glm::vec3 pos)> fill_function;

    NoiseSample(unsigned int precision, float scaleBy = 1);
    NoiseSample(glm::ivec2 precision, glm::vec2 scaleBy = {1, 1});

    void populate(const fill_function& fn);
    float get(glm::vec3 pos);
private:
    unsigned int length;

    std::vector<std::vector<std::vector<float>>> data {};
    glm::ivec3 precision {};
    glm::vec3 scaleBy;
};

