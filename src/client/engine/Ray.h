//
// Created by aurailus on 30/12/18.
//

#ifndef ZEUS_RAY_H
#define ZEUS_RAY_H

#include <vec3.hpp>
#include <cmath>
#include <glm.hpp>

#include "../gameworld/Player.h"

class Ray {
public:
    Ray() = default;
    explicit Ray(Player* player);
    Ray(glm::vec3 position, float yaw, float pitch);

    void step(float scale);
    glm::vec3* getEnd();

    float getLength();
private:
    glm::vec3 start;
    glm::vec3 position;
    float yaw;
    float pitch;
};


#endif //ZEUS_RAY_H
