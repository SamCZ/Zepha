//
// Created by aurailus on 08/02/19.
//

#pragma once

#include "../Entity.h"

class PlayerEntity : public Entity {
public:
    PlayerEntity(glm::vec3 pos, unsigned int cid, const std::shared_ptr<Model>& playerModel) : cid(cid) {
        setModel(playerModel);
        setPos(pos);
    }

    unsigned int getCid() {
        return cid;
    }

private:
    unsigned int cid;
};