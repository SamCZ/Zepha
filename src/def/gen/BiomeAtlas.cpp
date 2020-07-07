//
// Created by aurailus on 2019-11-13.
//

#include "BiomeAtlas.h"

#include "BiomeDef.h"

void BiomeAtlas::generateVoronoi() {
    std::vector<glm::vec3> points {};
    for (unsigned long i = 1; i < defs.size(); i++) {
        auto& biome = defs[i];

        points.emplace_back(
            static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, (biome->temperature + 1) / 2 * voronoiSize))),
            static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, biome->humidity * voronoiSize))),
            static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, biome->roughness * voronoiSize))));
    }
    voronoi.setPoints(points);
}

BiomeDef& BiomeAtlas::getBiomeAt(float temperature, float humidity, float roughness) {
    return biomeFromId(voronoi.getPoint(
        static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, (temperature + 1) / 2 * voronoiSize))),
        static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, humidity * voronoiSize))),
        static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, roughness * voronoiSize)))));
}

unsigned int BiomeAtlas::size() {
    return defs.size();
}

BiomeDef& BiomeAtlas::biomeFromId(unsigned int index) {
    if (index >= defs.size()) throw std::runtime_error("Undefined biome ID " + std::to_string(index) + " requested.");
    return *defs.at(static_cast<unsigned long>(index));
}

BiomeDef& BiomeAtlas::biomeFromStr(const std::string& identifier) {
    if (defTable.count(identifier) <= 0) throw std::runtime_error("Undefined biome identifier " + identifier + "requested.");
    return *defs.at(static_cast<unsigned long>(defTable.at(identifier)));
}
