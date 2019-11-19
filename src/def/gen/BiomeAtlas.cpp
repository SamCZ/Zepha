//
// Created by aurailus on 2019-11-13.
//

#include "BiomeAtlas.h"
#include "../../util/Log.h"

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

    voronoi.setColorValues({{0, 0, 0}, {0.5, 0.5, 0.5}, {1, 1, 1}, {0.7, 0.7, 0.7}, {0.3, 0.3, 0.3}, {0.15, 0.15, 0.15}});
    voronoi.generateImage(0);
    voronoi.generateImage(7);
    voronoi.generateImage(15);
    voronoi.generateImage(23);
    voronoi.generateImage(31);
    voronoi.generateImage(39);
    voronoi.generateImage(47);
    voronoi.generateImage(55);
    voronoi.generateImage(63);
}

BiomeDef& BiomeAtlas::getBiomeAt(float temperature, float humidity, float roughness) {
    return biomeFromId(voronoi.getPoint(
            static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, temperature + 1 / 2 * voronoiSize))),
            static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, humidity * voronoiSize))),
            static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, roughness * voronoiSize)))
    ));
}

unsigned int BiomeAtlas::size() {
    return defs.size();
}

BiomeDef& BiomeAtlas::biomeFromId(unsigned int index) {
    if (index >= defs.size()) {
        std::cout << Log::err << "Undefined biome #" << index << " requested! Throwing." << Log::endl;
        throw "Oops";
    }

    return *defs.at(static_cast<unsigned long>(index));
}

BiomeDef& BiomeAtlas::biomeFromStr(const std::string& identifier) {
    if (defTable.count(identifier) <= 0) {
        std::cout << Log::err << "Undefined identifier \"" << identifier << "\" requested! Throwing." << Log::endl;
        throw "Oops";
    }

    return *defs.at(static_cast<unsigned long>(defTable.at(identifier)));
}
