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
class Deserializer;
enum class NetPlayerField;

class Player : Collidable, public Drawable {
public:
    static constexpr float MOUSE_SENSITIVITY = 0.1f;
    static constexpr float LOOK_DISTANCE = 6.5f;
    static constexpr float LOOK_PRECISION = 0.01f;
    static constexpr float EYE_HEIGHT = 1.65f;
    static constexpr float BASE_MOVE_SPEED = 4.3f;
    static constexpr float JUMP_VEL = 0.14f;

    Player(LocalWorld& world, ClientGame& defs, Renderer& renderer, LocalInventoryRefs& refs);
    void update(Input &input, double delta, glm::vec2 mouseDelta);
    ~Player();

    glm::vec3 getPos();
    void setPos(glm::vec3 pos, bool assert = false);

    glm::vec3 getVel();
    void setVel(glm::vec3 vel, bool assert = false);

    float getYaw();
    void setYaw(float yaw, bool assert = false);

    float getPitch();
    void setPitch(float pitch, bool assert = false);

    bool isFlying();
    void setFlying(bool flying, bool assert = false);

    LocalInventory& getInventory();

    std::shared_ptr<LocalInventoryList> getHandList();
    void setHandList(const std::string& list);

    std::shared_ptr<LocalInventoryList> getWieldList();
    void setWieldList(const std::string& list, bool assert = false);

    unsigned short getWieldIndex();
    void setWieldIndex(unsigned short index, bool assert = false);

    bool isInMenu();
    void showMenu(std::shared_ptr<LuaGuiElement> root);
    void closeMenu();

    void setHud(std::shared_ptr<LuaGuiElement> hud);
    std::shared_ptr<LuaGuiElement> getHud();

    PointedThing& getPointedThing();
    void setHudVisible(bool hudVisible);

    void draw(Renderer& renderer) override;
    void drawHud(Renderer& renderer);
    void drawMenu(Renderer& renderer);

    void handleAssertion(Deserializer& d);

    unsigned int id = 0;
private:
    void moveAndLook(Input &input, double delta, glm::vec2 mouseDelta);
    void updateCamera();
    void findPointedThing(Input &input);
    void updateWireframe();
    void interact(Input& input, double delta);

    void updateWieldAndHandItems();
    template <typename T> void assertField(NetPlayerField field, T data);

    ClientGame& game;
    Renderer& renderer;

    GameGui gameGui;
    Entity handModel;
    Entity handItemModel;
    WireframeEntity wireframe;

    LocalInventoryRefs& refs;
    unsigned int wieldIndex = 0;

    unsigned int handItem = DefinitionAtlas::AIR;
    unsigned int wieldItem = DefinitionAtlas::AIR;

    float yaw = 0;
    float pitch = 0;
    bool flying = false;

    double breakTime = 0;
    double breakInterval = 0;
    PointedThing pointedThing;
};

