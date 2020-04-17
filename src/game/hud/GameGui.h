//
// Created by aurailus on 05/02/19.
//

#pragma once

#include "GameGuiBuilder.h"
#include "SerialGui.h"
#include "components/basic/GuiText.h"
#include "components/basic/GuiRect.h"
#include "components/basic/GuiContainer.h"
#include "../graph/drawable/DrawableGroup.h"
#include "../inventory/ServerInventoryList.h"
#include "../entity/Entity.h"
#include "../../util/Util.h"
#include "components/compound/GuiInventoryList.h"
#include "../inventory/Inventory.h"

class GameGui {
public:
    explicit GameGui(LocalInventoryRefs& refs, glm::vec2 bufferSize, ClientGame& defs, Renderer& renderer);
    void winResized(glm::ivec2 win);
    void update(double delta);

    void setVisible(bool visible);
    bool isVisible();

    void showMenu(std::shared_ptr<LuaGuiElement> root);
    void closeMenu();
    const bool isInMenu() const;

    void setHud(std::shared_ptr<LuaGuiElement> hud);
    std::shared_ptr<LuaGuiElement> getHud();

    void drawHud(Renderer& renderer);
    void drawMenu(Renderer& renderer);
private:
    ClientGame& defs;
    Renderer& renderer;

    glm::ivec2 win {};
    bool inMenu = false;

    std::shared_ptr<LuaGuiElement> hudRootElem = nullptr;

    std::shared_ptr<GuiContainer> menuRoot = std::make_shared<GuiInventoryList>("menuRoot");
    std::shared_ptr<GuiContainer> menuLuaRoot = std::make_shared<GuiInventoryList>("menuLuaRoot");
    GameGuiBuilder menuBuilder;
    std::shared_ptr<GuiContainer> hudRoot  = std::make_shared<GuiInventoryList>("hudRoot");
    std::shared_ptr<GuiContainer> hudLuaRoot  = std::make_shared<GuiInventoryList>("hudLuaRoot");
    GameGuiBuilder hudBuilder;

    std::shared_ptr<GuiInventoryList> handList = std::make_shared<GuiInventoryList>("hand");

    LocalInventoryRefs& refs;
};

