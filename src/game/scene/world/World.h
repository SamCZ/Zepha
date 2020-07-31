//
// Created by aurailus on 2020-01-09.
//

#pragma once

#include <memory>
#include <glm/vec3.hpp>
#include <unordered_map>

#include "../../../util/Vec.h"

class Subgame;
class Dimension;

class World {
public:
    World(const World& o) = delete;
    explicit World(Subgame& game);

    virtual void update(double delta);

    virtual Dimension& createDimension(const std::string& identifier) = 0;

    virtual Dimension& getDefaultDimension();
    virtual void setDefaultDimension(const std::string& defaultDimension);

    virtual Dimension& getDimension(unsigned int index) = 0;
    virtual Dimension& getDimension(const std::string& identifier) = 0;

protected:
    std::string defaultDimension {};
    std::vector<std::shared_ptr<Dimension>> dimensions;

    struct Damage { double curr, max; };
    std::unordered_map<glm::ivec3, Damage, Vec::ivec3> blockDamages;

    Subgame& game;
};
