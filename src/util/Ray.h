//
// Created by aurailus on 30/12/18.
//

#ifndef ZEUS_RAY_H
#define ZEUS_RAY_H

#include <cmath>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include "../game/scene/world/Player.h"

class Ray {
public:
    Ray() = default;
    explicit Ray(Player* player);
    Ray(glm::vec3 pos, float yaw, float pitch);

    void step(float scale);
    glm::vec3* getEnd();

    float getLength();
private:
    glm::vec3 start;
    glm::vec3 pos;
    glm::vec3 dir;
};


#endif //ZEUS_RAY_H
