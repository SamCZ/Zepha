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

class Player : public Drawable {
public:
    static constexpr float LOOK_DISTANCE = 6.5f;
    static constexpr float LOOK_PRECISION = 0.01f;
    static constexpr float EYE_HEIGHT = 1.65f;

    Player(LocalWorld& world, LocalDefs& defs, Renderer& renderer);

    void update(InputManager &input, double delta, double mouseX, double mouseY);

    void posUpdate(InputManager &input, double delta);
    void viewUpdate(double deltaX, double deltaY);
    void pointerUpdate(InputManager &input, double delta);

    bool collides(glm::vec3 pos);
    void moveCollide();

    void setPos(glm::vec3 pos);
    glm::vec3 getPos();

    void setVel(glm::vec3 vel);
    glm::vec3 getVel();

    void setYaw(float yaw);
    float getYaw();
    void setPitch(float pitch);
    float getPitch();

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
    LocalWorld& world;
    GameGui gameGui;

    glm::vec3 pos {};
    glm::vec3 vel {};
    bool flying = false;
    float yaw = 0;
    float pitch = 0;

    PointedThing pointedThing;
    WireframeEntity wireframe;
    double breakInterval = 0;
};

