//
// Created by aurailus on 08/02/19.
//

#pragma once

#include "../DrawableEntity.h"

class PlayerEntity : public DrawableEntity {
public:
    PlayerEntity(SubgamePtr game, DimensionPtr dim, glm::vec3 pos, unsigned int id, const std::shared_ptr<Model>& model) :
        DrawableEntity(game, dim, model), Entity(game, dim), id(id) {
        setPos(pos);
    }

    unsigned int getId() {
        return id;
    }

private:
    unsigned int id;
};