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
#include "../../entity/world/BlockModelEntity.h"
#include "../../graph/drawable/DrawableGroup.h"

class Player {
public:
    static constexpr float LOOK_DISTANCE = 6.5f;
    static constexpr float EYE_HEIGHT = 1.65f;

    Player() = default;
    Player(LocalWorld* world, GameDefs* defs, Camera* camera, WireframeEntity* wireframe, DrawableGroup* blockBreakEntities);

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

    ~Player() = default;

    bool pointingAtBlock = false;
    float digPercentage = 0;
    glm::vec3 pointedBlock {};
private:
    glm::vec3 pos {};
    float yaw = 0;
    float pitch = 0;
    glm::vec3 vel {};

    Camera* camera = nullptr;
    GameDefs* defs = nullptr;
    LocalWorld* world = nullptr;

    WireframeEntity* wireframe = nullptr;
    BlockModelEntity* currentBlockBreak = nullptr;
    DrawableGroup* blockBreakEntities = nullptr;
    SelectionBox box;

    float crackLevel = 0;

    bool flying = false;
};


#endif //ZEUS_PLAYER_H
