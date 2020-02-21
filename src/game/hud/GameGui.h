//
// Created by aurailus on 05/02/19.
//

#pragma once

#include "GameGuiBuilder.h"
#include "SerialGui.h"
#include "components/basic/GUIText.h"
#include "components/basic/GUIRect.h"
#include "components/basic/GUIContainer.h"
#include "../graph/drawable/DrawableGroup.h"
#include "../scene/world/InventoryList.h"
#include "../entity/Entity.h"
#include "../../util/Util.h"
#include "components/compound/GUIInventoryList.h"
#include "../scene/world/Inventory.h"

class GameGui : public GUIContainer {
public:
    explicit GameGui(Inventory& inventory, InventoryList& hand, glm::vec2 bufferSize, ClientGame& defs, Renderer& renderer);
    void winResized(glm::ivec2 win);

    void update(double delta) override;
    void setVisible(bool visible) override;

    void setMenu(const std::string& menu, const std::map<std::string, GuiBuilder::ComponentCallbacks>& callbacks);
    void closeMenu();
    const std::string& getMenuState();

    void drawViginette(Renderer& renderer);
private:
    ClientGame& defs;
    Renderer& renderer;

    glm::ivec2 win {};
    std::string menuState = "";

    std::shared_ptr<GUIContainer> menuRoot = {};
    std::shared_ptr<GUIInventoryList> handList = {};
    GUIContainer builtIn = {};
    GameGuiBuilder builder;

    InventoryList& hand;
    Inventory& inventory;
};

