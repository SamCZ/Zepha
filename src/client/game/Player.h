//
// Created by aurailus on 28/12/18.
//

#ifndef ZEUS_PLAYER_H
#define ZEUS_PLAYER_H


#include <iostream>

#include "localworld/LocalWorld.h"
#include "../engine/Camera.h"
#include "../engine/Timer.h"
#include "entity/WireframeGenerator.h"

class Player {
public:
    static constexpr float LOOK_DISTANCE = 6.5f;
    static constexpr float EYE_HEIGHT = 1.65f;

    Player();

    void create(LocalWorld* world, Camera* camera, Entity* wireframe);

    void update(InputManager &input, double delta, double mouseX, double mouseY);

    void posUpdate(InputManager &input, double delta);
    void viewUpdate(double deltaX, double deltaY);
    bool collides(glm::vec3 pos);
    void moveCollide();

    glm::vec3* getPos();
    void setPos(glm::vec3 pos);

    float getYaw();
    float getPitch();

    glm::vec3* getVel();
    void setVel(glm::vec3 vel);

    glm::vec3* getPointedBlock();

    ~Player();

    bool pointingAtBlock;
    float digPercentage;
    glm::vec3 pointedBlock;
private:
    glm::vec3 pos;
    float yaw, pitch;
    glm::vec3 vel;

    Camera* camera;
    LocalWorld* world;

    Entity* wireframe;
    SelectionBox box;

    bool flying;
};


#endif //ZEUS_PLAYER_H
