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
    typedef std::function<float(glm::ivec3 pos)> fill_function;

    NoiseSample() = default;

    void fill(const fill_function& fun, float precision);
    void fill(const fill_function& fun, glm::ivec2 precision);

    float get(glm::ivec3 localPos);
    void  set(glm::ivec3 localPos, float value);
private:
    void reserve();

    std::vector<std::vector<std::vector<float>>> data {};
    glm::ivec2 precision {};
};

