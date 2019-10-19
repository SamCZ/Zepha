//
// Created by aurailus on 08/02/19.
//

#pragma once

#include <memory>
#include "../Entity.h"
#include "../../../def/texture/AtlasRef.h"

class PlayerEntity : public Entity {
public:
    PlayerEntity(glm::vec3 pos, int peer_id, const std::shared_ptr<Model>& playerModel);
    int peer_id;
};
