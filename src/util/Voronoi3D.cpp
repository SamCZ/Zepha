//
// Created by aurailus on 2019-11-12.
//

#include "Voronoi3D.h"
#include <glm/glm.hpp>

Voronoi3D::Voronoi3D(unsigned short size) :
    size(size),
    data(voronoi_data(size)) {

    for (unsigned short i = 0; i < size; i++) {
        data[i].resize(size);
        for (unsigned short j = 0; j < size; j++) {
            data[i][j].resize(size);
        }
    }
}

void Voronoi3D::setPoints(const std::vector<glm::vec3>& points) {
    this->points = points;

    for (unsigned short i = 0; i < size; i++) {
        for (unsigned short j = 0; j < size; j++) {
            for (unsigned short k = 0; k < size; k++) {

                float pointDistance = INFINITY;
                unsigned short pIndex = 0;

                for (unsigned short p = 0; p < points.size(); p++) {
                    float thisPointDistance = glm::distance(points[p], {i, j, k});
                    if (thisPointDistance < pointDistance) {
                        pointDistance = thisPointDistance;
                        pIndex = p;
                    }
                }

                data[i][j][k] = pIndex + 1; //Note: This is because of the INVALID biome
            }
        }
    }
}

unsigned short Voronoi3D::getPoint(unsigned short x, unsigned short y, unsigned short z) {
    return data[x][y][z];
}

void Voronoi3D::setColorValues(const std::vector<glm::vec3>& values) {
    colorValues = values;
}

void Voronoi3D::generateImage(unsigned short depth) {
    auto colorData = new unsigned char[size * size * 3];

    for (int i = 0; i < size * size; i++) {
        int x = i % size;
        int y = i / size;

        unsigned short point = data[x][y][depth];
        auto color = colorValues[point];

        colorData[i * 3 + 0] = static_cast<int>(color.x * 255.f);
        colorData[i * 3 + 1] = static_cast<int>(color.y * 255.f);
        colorData[i * 3 + 2] = static_cast<int>(color.z * 255.f);
    }

    std::string name = "voronoi_";
    name += std::to_string(depth) + ".jpg";
    stbi_write_jpg(name.data(), size, size, 3, colorData, 100);
}