//
// Created by aurailus on 28/12/18.
//

#pragma once


#include <iostream>

#include "LocalWorld.h"
#include "Inventory.h"
#include "../../hud/GameGui.h"
#include "../../graph/Camera.h"
#include "../../graph/drawable/DrawableGroup.h"
#include "../../entity/Collidable.h"
#include "../../entity/world/WireframeEntity.h"
#include "../../entity/world/BlockCrackEntity.h"
#include "../../../util/Ray.h"
#include "../../../util/Timer.h"
#include "../../../world/block/PointedThing.h"

class Player : Collidable, public Drawable {
public:
    static constexpr float MOUSE_SENSITIVITY = 0.1f;
    static constexpr float LOOK_DISTANCE = 6.5f;
    static constexpr float LOOK_PRECISION = 0.01f;
    static constexpr float EYE_HEIGHT = 1.65f;
    static constexpr float BASE_MOVE_SPEED = 4.3f;
    static constexpr float JUMP_VEL = 0.14f;
    static constexpr float BLOCK_DAMAGE = 0.36f;
    static constexpr float BLOCK_INTERVAL = 0.02f;

    Player(LocalWorld& world, ClientGame& defs, Renderer& renderer);
    void update(Input &input, double delta, glm::vec2 mouseDelta);
    ~Player();

    void setPos(glm::vec3 pos);
    glm::vec3 getPos();

    void setVel(glm::vec3 vel);
    glm::vec3 getVel();

    void setYaw(float yaw);
    float getYaw();

    void setPitch(float pitch);
    float getPitch();

    void setFlying(bool flying);
    bool isFlying();

    void setActiveBlock(const std::string& block);

    void setMenu(const std::string& menu, const std::map<std::string, GuiBuilder::ComponentCallbacks>& callbacks);
    std::string getMenuState();
    void closeMenu();
    void setGuiVisible(bool hudVisible);

    void draw(Renderer& renderer) override;
    void drawGUI(Renderer& renderer);
    void drawViginette(Renderer& renderer);

    Inventory& getInventory();
    PointedThing& getPointedThing();
private:
    void moveAndLook(Input &input, double delta, glm::vec2 mouseDelta);
    void updateCamera();
    void findPointedThing(Input &input);
    void updateWireframe();
    void breakBlock(Input& input, double delta);

    Renderer& renderer;
    ClientGame& defs;

    InventoryList hand;
    Inventory inventory;
    GameGui gameGui;

    float yaw = 0;
    float pitch = 0;
    bool flying = false;

    unsigned int activeBlock = -1;

    PointedThing pointedThing;
    WireframeEntity wireframe;
    Entity handModel;
    double breakInterval = 0;
};

