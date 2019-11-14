//
// Created by aurailus on 2019-11-12.
//

#pragma once

#include <vector>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <string>

#include "stb_image/stb_image_write.h"

class Voronoi3D {
public:
    Voronoi3D() = default;
    Voronoi3D(unsigned short size);

    void setPoints(const std::vector<glm::vec3>& points);

    void setColorValues(const std::vector<glm::vec3>& values);
    void generateImage(unsigned short depth);
private:
    typedef std::vector<std::vector<std::vector<unsigned short>>> voronoi_data;

    unsigned short size;
    voronoi_data data;
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> colorValues;
};
