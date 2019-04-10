//
// Created by aurailus on 08/02/19.
//

#ifndef ZEUS_PLAYERENTITY_H
#define ZEUS_PLAYERENTITY_H


#include "../../engine/Entity.h"

class PlayerEntity : public Entity {
public:
    PlayerEntity(glm::vec3 pos, int peer_id);
    ~PlayerEntity() override;

    int peer_id;
private:
    Texture* crosshairTexture;
};


#endif //ZEUS_PLAYERENTITY_H
