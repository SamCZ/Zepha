//
// Created by aurailus on 2020-01-09.
//

#pragma once

#include <memory>
#include <glm/vec3.hpp>
#include <unordered_map>

#include "../../../util/Vec.h"
#include "../../../util/CovariantPtr.h"

class Subgame;
class Dimension;

class World {
public:
    World(const World& o) = delete;
    explicit World(SubgamePtr game);

    virtual void update(double delta);

    virtual DimensionPtr createDimension(const std::string& identifier) = 0;

    virtual DimensionPtr getDefaultDimension();
    virtual void setDefaultDimension(const std::string& defaultDimension);

    virtual DimensionPtr getDimension(unsigned int index);
    virtual DimensionPtr getDimension(const std::string& identifier);

    virtual InventoryRefsPtr getRefs() = 0;
protected:
    std::string defaultDimension {};
    std::vector<std::shared_ptr<Dimension>> dimensions;

    SubgamePtr game;
};
