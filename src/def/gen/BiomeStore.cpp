//
// Created by aurailus on 2019-11-13.
//

#include "BiomeStore.h"
#include "../../util/Log.h"

void BiomeStore::registerBiome(BiomeDef *def) {
    defs.push_back(def);
    defTable.insert({def->identifier, def->index});
}

void BiomeStore::generateVoronoi() {
    std::vector<glm::vec3> points {};
    for (auto biome : defs) {
        points.emplace_back(
                static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, (biome->temperature + 1) / 2 * voronoiSize))),
                static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, biome->humidity * voronoiSize))),
                static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, biome->roughness * voronoiSize))));
    }
    voronoi.setPoints(points);

    voronoi.setColorValues({{0, 0, 0}, {0.5, 0.5, 0.5}, {1, 1, 1}});
    voronoi.generateImage(0);
    voronoi.generateImage(8);
    voronoi.generateImage(16);
    voronoi.generateImage(24);
    voronoi.generateImage(32);
    voronoi.generateImage(40);
    voronoi.generateImage(48);
    voronoi.generateImage(56);
    voronoi.generateImage(63);
}

BiomeDef& BiomeStore::getBiomeAt(float temperature, float humidity, float roughness) {
    return biomeFromId(voronoi.getPoint(
            static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, temperature + 1 / 2 * voronoiSize))),
            static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, humidity * voronoiSize))),
            static_cast<unsigned short>(std::fmin(voronoiSize - 1, std::fmax(0, roughness * voronoiSize)))
    ));
}

unsigned int BiomeStore::size() {
    return defs.size();
}

BiomeDef& BiomeStore::biomeFromId(unsigned int index) {
    if (index >= defs.size()) {
        std::cout << Log::err << "Undefined biome #" << index << " requested! Throwing." << Log::endl;
        throw "Oops";
    }

    return *defs.at(static_cast<unsigned long>(index));
}

BiomeDef& BiomeStore::biomeFromStr(const std::string& identifier) {
    if (defTable.count(identifier) <= 0) {
        std::cout << Log::err << "Undefined identifier \"" << identifier << "\" requested! Throwing." << Log::endl;
        throw "Oops";
    }

    return *defs.at(static_cast<unsigned long>(defTable.at(identifier)));
}
