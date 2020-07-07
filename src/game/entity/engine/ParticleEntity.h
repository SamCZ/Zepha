//
// Created by aurailus on 15/05/19.
//

#pragma once

#include "../Entity.h"

class BlockDef;

class ParticleEntity : public Entity {
public:
    ParticleEntity(glm::vec3 pos, BlockDef& block);

    void update(double delta, glm::vec3 player);
    void draw(Renderer& renderer) override;

    float time = 0;
private:
    glm::vec3 velocity {};
};

