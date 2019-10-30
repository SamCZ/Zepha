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
#include "../../hud/GameGui.h"
#include "../../entity/Collidable.h"

class Player : Collidable, public Drawable {
public:
    static constexpr float MOUSE_SENSITIVITY = 0.1f;
    static constexpr float LOOK_DISTANCE = 6.5f;
    static constexpr float LOOK_PRECISION = 0.01f;
    static constexpr float EYE_HEIGHT = 1.65f;
    static constexpr float BASE_MOVE_SPEED = 7.5f;
    static constexpr float JUMP_VEL = 0.15f;
    static constexpr float BLOCK_DAMAGE = 0.45f;
    static constexpr float BLOCK_INTERVAL = 0.33f;

    Player(LocalWorld& world, LocalDefs& defs, Renderer& renderer);

    void update(InputManager &input, double delta, double mouseX, double mouseY);

    void moveAndLook(InputManager &input, double delta, double deltaX, double deltaY);
    void updateCamera();
    void findPointedThing(InputManager &input);
    void updateWireframe();
    void breakBlock(InputManager& input, double delta);

    void setPos(glm::vec3 pos);
    glm::vec3 getPos();

    void setVel(glm::vec3 vel);
    glm::vec3 getVel();

    void setYaw(float yaw);
    float getYaw();
    void setPitch(float pitch);
    float getPitch();

    void setActiveBlock(const std::string& block);

    GameGui& getGui();
    void setMenu(const std::string& state, sptr<GUIComponent> root);
    void closeMenu();
    void setGuiVisible(bool hudVisible);

    void draw(Renderer& renderer) override;
    void drawGUI(Renderer& renderer);

    PointedThing& getPointedThing();

private:
    Renderer& renderer;
    LocalDefs& defs;
    GameGui gameGui;

    bool flying = false;
    float yaw = 0;
    float pitch = 0;

    unsigned int activeBlock = -1;

    PointedThing pointedThing;
    WireframeEntity wireframe;
    double breakInterval = 0;
};

