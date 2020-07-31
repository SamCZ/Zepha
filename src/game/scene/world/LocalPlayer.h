//
// Created by aurailus on 28/12/18.
//

#pragma once

#include "Player.h"
#include "../../graph/drawable/Drawable.h"

#include "../../hud/GameGui.h"
#include "../../../world/Target.h"
#include "../../../world/LocalDimension.h"
#include "../../entity/engine/WireframeEntity.h"

class Input;
class Deserializer;
class LuaGuiElement;
class LocalInventory;
class LocalInventoryRefs;
enum class NetPlayerField;

class LocalPlayer : public virtual DrawableEntity, public Player {
public:
    enum class PlayerControl { FORWARD, LEFT, BACKWARD, RIGHT, JUMP, MOD1, MOD2 };

    LocalPlayer(LocalSubgame &game, LocalDimension& dim, Renderer &renderer);

    void update(Input &input, double delta, glm::vec2 mouseDelta);
    virtual void assertField(Packet packet) override;
    virtual void handleAssertion(Deserializer& d) override;

    virtual LocalInventory& getInventory() override;
    virtual LocalDimension& getDimension() override;

    virtual void setPos(glm::vec3 pos, bool assert = false) override;
    virtual void setLookOffset(glm::vec3 eyeOffset, bool assert = false) override;
    virtual void setHandList(const std::string& list, bool assert = false) override;
    virtual void setWieldList(const std::string& list, bool assert = false) override;
    virtual void setWieldIndex(unsigned short index, bool assert = false) override;

    bool isInMenu();
    void showMenu(std::shared_ptr<LuaGuiElement> root);
    void closeMenu();

    void setHud(std::shared_ptr<LuaGuiElement> hud);
    std::shared_ptr<LuaGuiElement> getHud();
    void setHudVisible(bool hudVisible);

    Target& getPointedThing();

    void draw(Renderer& renderer) override;
    void drawHud(Renderer& renderer);
    void drawMenu(Renderer& renderer);

    ~LocalPlayer();

private:
    bool getKey(Input& input, PlayerControl control);

    void updatePhysics(Input &input, double delta, glm::vec2 mouseDelta);
    void updateCamera();

    void findPointedThing(Input &input);
    void updateWireframe();

    void interact(Input& input, double delta);

    void updateWieldAndHandItems();

    GameGui gameGui;
    DrawableEntity handModel;
    DrawableEntity handItemModel;
    WireframeEntity wireframe;

    Renderer& renderer;

    double breakTime = 0;
    double breakInterval = 0;
    Target target;
};

