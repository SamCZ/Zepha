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
#include "../scene/world/InventoryList.h"
#include "../entity/Entity.h"
#include "../../util/Util.h"
#include "components/compound/GuiInventoryList.h"
#include "../scene/world/Inventory.h"

class GameGui : public GuiContainer {
public:
    explicit GameGui(LocalInventoryRefs& refs, glm::vec2 bufferSize, ClientGame& defs, Renderer& renderer);
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

    std::shared_ptr<GuiContainer> menuRoot = {};
    std::shared_ptr<GuiInventoryList> handList = {};
    GuiContainer builtIn = {};
    GameGuiBuilder builder;

    LocalInventoryRefs& refs;
};

