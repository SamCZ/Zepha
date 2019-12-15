//
// Created by aurailus on 05/02/19.
//

#pragma once

#include "GameGuiBuilder.h"
#include "SerializedGuiElem.h"
#include "components/basic/GUIText.h"
#include "components/basic/GUIRect.h"
#include "components/basic/GUIContainer.h"
#include "../graph/drawable/DrawableGroup.h"
#include "../scene/world/InventoryList.h"
#include "../entity/Entity.h"
#include "../../util/Util.h"
#include "components/compound/GUIInventoryList.h"

class GameGui : public GUIContainer {
public:
    explicit GameGui(glm::vec2 bufferSize, LocalDefs& defs, Renderer& renderer);
    void winResized(glm::ivec2 win);

    void update(double delta) override;
    void setVisible(bool visible) override;

    void setMenu(const std::string& menu, const std::map<std::string, std::function<void(glm::ivec2)>>& callbacks);
    void closeMenu();
    const std::string& getMenuState();

    void drawViginette(Renderer& renderer);

    InventoryList list, hand;
private:
    LocalDefs& defs;
    Renderer& renderer;

    glm::ivec2 win {};
    std::string menuState = "";

    GUIContainer builtIn = {};
    std::shared_ptr<GUIContainer> menuRoot = {};
    std::shared_ptr<GUIInventoryList> handList = {};
    GameGuiBuilder builder;
};

