//
// Created by aurailus on 28/12/18.
//

#ifndef ZEUS_PLAYER_H
#define ZEUS_PLAYER_H


#include <iostream>

#include "LocalWorld.h"
#include "../../graph/Camera.h"
#include "../../../util/Timer.h"
#include "../../entity/world/WireframeEntity.h"
#include "../../entity/world/BlockCrackEntity.h"
#include "../../graph/drawable/DrawableGroup.h"
#include "../../../world/block/PointedThing.h"

class Player {
public:
    static constexpr float LOOK_DISTANCE = 6.5f;
    static constexpr float LOOK_PRECISION = 0.01f;
    static constexpr float EYE_HEIGHT = 1.65f;

    Player() = default;
    Player(LocalWorld* world, GameDefs* defs, Camera* camera, WireframeEntity* wireframe);

    void update(InputManager &input, double delta, double mouseX, double mouseY);

    void posUpdate(InputManager &input, double delta);
    void viewUpdate(double deltaX, double deltaY);
    void pointerUpdate(InputManager &input, double delta);

    bool collides(glm::vec3 pos);
    void moveCollide();

    glm::vec3 getPos();
    void setPos(glm::vec3 pos);

    float getYaw();
    float getPitch();

    glm::vec3 getVel();
    void setVel(glm::vec3 vel);

    PointedThing& getPointedThing();

    ~Player() = default;

    double breakInterval = 0;
private:
    glm::vec3 pos {};
    glm::vec3 vel {};
    float yaw = 0;
    float pitch = 0;

    Camera* camera = nullptr;
    GameDefs* defs = nullptr;
    LocalWorld* world = nullptr;

    PointedThing pointedThing;

    WireframeEntity* wireframe = nullptr;

    bool flying = false;
};


#endif //ZEUS_PLAYER_H
