//
// Created by aurailus on 28/12/18.
//

#ifndef ZEUS_PLAYER_H
#define ZEUS_PLAYER_H


#include "../../engine/Camera.h"
#include "World.h"

class Player {
public:
    Player();

    void create(World* world, Camera* camera);

    void update(bool* keys, GLfloat delta, double mouseX, double mouseY);

    void posUpdate(bool *keys, GLfloat delta);
    void viewUpdate(double deltaX, double deltaY);
    bool collides(glm::vec3 pos);
    void moveCollide();

    glm::vec3* getPos();
    void setPos(glm::vec3 pos);

    float getYaw();
    float getPitch();

    glm::vec3* getVel();
    void setVel(glm::vec3 vel);

    ~Player();
private:
    glm::vec3 pos;
    float yaw, pitch;
    glm::vec3 vel;

    Camera* camera;
    World* world;

    const float playerHeight = 1.6;
};


#endif //ZEUS_PLAYER_H
