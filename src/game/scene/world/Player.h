//
// Created by aurailus on 28/12/18.
//

#pragma once


#include <iostream>

#include "LocalWorld.h"
#include "../../graph/Camera.h"
#include "../../../util/Timer.h"
#include "../../entity/world/WireframeEntity.h"
#include "../../entity/world/BlockCrackEntity.h"
#include "../../graph/drawable/DrawableGroup.h"
#include "../../../world/block/PointedThing.h"

class Player : public Drawable {
public:
    static constexpr float LOOK_DISTANCE = 6.5f;
    static constexpr float LOOK_PRECISION = 0.01f;
    static constexpr float EYE_HEIGHT = 1.65f;

    Player(LocalWorld& world, LocalDefs& defs, Camera& camera);

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

    void draw(Renderer& renderer) override;

    PointedThing& getPointedThing();
private:
    Camera& camera;
    LocalDefs& defs;
    LocalWorld& world;

    glm::vec3 pos {};
    glm::vec3 vel {};
    bool flying = false;
    float yaw = 0;
    float pitch = 0;

    PointedThing pointedThing;
    WireframeEntity wireframe;
    double breakInterval = 0;
};

