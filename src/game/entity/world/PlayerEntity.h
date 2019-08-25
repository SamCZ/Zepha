//
// Created by aurailus on 08/02/19.
//

#ifndef ZEUS_PLAYERENTITY_H
#define ZEUS_PLAYERENTITY_H

#include <memory>
#include "Entity.h"
#include "../../../def/texture/AtlasRef.h"

class PlayerEntity : public Entity {
public:
    PlayerEntity(glm::vec3 pos, int peer_id, std::shared_ptr<AtlasRef> playerFrontTex, std::shared_ptr<AtlasRef> playerBackTex, std::shared_ptr<AtlasRef> shadowTex);

    int peer_id;
private:
    std::shared_ptr<AtlasRef> playerFrontTex, playerBackTex, shadowTex;
};

#endif //ZEUS_PLAYERENTITY_H
