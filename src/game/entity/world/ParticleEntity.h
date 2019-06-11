//
// Created by aurailus on 15/05/19.
//

#ifndef ZEUS_PARTICLEENTITY_H
#define ZEUS_PARTICLEENTITY_H


#include "../Entity.h"
#include "../../../def/block/client/LocalBlockDef.h"

class ParticleEntity : public Entity {
public:
    ParticleEntity(glm::vec3 pos, LocalBlockDef& block);

    void update(double delta, glm::vec3 player);
    void draw(Renderer& renderer) override;

    float time = 0;
private:
    glm::vec3 velocity {};
};


#endif //ZEUS_PARTICLEENTITY_H
