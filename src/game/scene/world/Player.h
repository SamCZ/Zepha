//
// Created by aurailus on 28/12/18.
//

#pragma once

#include "../../entity/Collidable.h"
#include "../../graph/drawable/Drawable.h"

#include "../../hud/GameGui.h"
#include "../../../world/PointedThing.h"
#include "../../entity/engine/WireframeEntity.h"

class Input;
class LuaGuiElement;
class LocalInventory;
class LocalInventoryRefs;

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

    Player(LocalWorld& world, ClientGame& defs, Renderer& renderer, LocalInventoryRefs& refs);
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

    void showMenu(std::shared_ptr<LuaGuiElement> root);
    void closeMenu();
    bool isInMenu();

    void setHud(std::shared_ptr<LuaGuiElement> hud);
    std::shared_ptr<LuaGuiElement> getHud();

    void setHudVisible(bool hudVisible);

    void draw(Renderer& renderer) override;
    void drawHud(Renderer& renderer);
    void drawMenu(Renderer& renderer);

    LocalInventory& getInventory();
    PointedThing& getPointedThing();
private:
    void moveAndLook(Input &input, double delta, glm::vec2 mouseDelta);
    void updateCamera();
    void findPointedThing(Input &input);
    void updateWireframe();
    void breakBlock(Input& input, double delta);

    ClientGame& game;
    Renderer& renderer;
    LocalInventoryRefs& refs;

    GameGui gameGui;
    Entity handModel;
    Entity handItemModel;
    WireframeEntity wireframe;

    float yaw = 0;
    float pitch = 0;
    bool flying = false;

    unsigned int activeBlock = DefinitionAtlas::AIR;
    PointedThing pointedThing;
    double breakInterval = 0;
};

