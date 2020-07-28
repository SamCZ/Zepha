//
// Created by aurailus on 2020-01-09.
//

#pragma once

#include <map>
#include <memory>
#include <glm/vec3.hpp>
#include <unordered_map>

#include "../../../util/Vec.h"

class Subgame;
class Dimension;

class World {
public:
    explicit World(Subgame& game);

    virtual void update(double delta) = 0;

//    virtual Dimension& getDimension() = 0;

    virtual unsigned int getBlock(glm::ivec3 pos) = 0;
    virtual void setBlock(glm::ivec3 pos, unsigned int block) = 0;

    virtual double getBlockDamage(glm::ivec3 pos) const;
    virtual double setBlockDamage(glm::ivec3 pos, double damage);

protected:
    void updateBlockDamages();

    std::map<std::string, std::shared_ptr<Dimension>> dimensions;

    struct Damage { double curr, max; };
    std::unordered_map<glm::ivec3, Damage, Vec::ivec3> blockDamages;

    Subgame& game;
};
