//
// Created by aurailus on 2020-01-09.
//

#include <unordered_map>

#include "World.h"

#include "world/dim/Dimension.h"

World::World(SubgamePtr game) : game(game) {}

void World::update(double delta) {
    for (auto& dimension : dimensions) dimension->update(delta);
}

DimensionPtr World::getDefaultDimension() {
    if (defaultDimension.empty()) throw std::runtime_error("No default dimension was set.");
    return getDimension(defaultDimension);
}

void World::setDefaultDimension(const std::string& identifier) {
    defaultDimension = identifier;
}

DimensionPtr World::getDimension(unsigned int index) {
    return dimensions[index];
}

DimensionPtr World::getDimension(const std::string &identifier) {
    for (auto& dimension : dimensions) if (dimension->getIdentifier() == identifier) return dimension;
    throw std::runtime_error("No dimension named " + identifier + " found.");
}
