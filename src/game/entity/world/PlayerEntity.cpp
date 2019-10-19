//
// Created by aurailus on 08/02/19.
//

#include "PlayerEntity.h"

PlayerEntity::PlayerEntity(glm::vec3 pos, int peer_id, const std::shared_ptr<Model>& playerModel) :
    peer_id(peer_id) {

    setModel(playerModel);
    setPos(pos);
}