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

class GameGui : public GUIContainer {
public:
    explicit GameGui(glm::vec2 bufferSize, LocalDefs& defs);
    void winResized(glm::ivec2 win);

    void setVisible(bool visible) override;

    void setMenu(const std::string& menu);
    void closeMenu();
    const std::string& getMenuState();

    void drawViginette(Renderer& renderer);

    InventoryList list;
private:
    glm::ivec2 win {};
    LocalDefs& defs;
    std::string menuState = "";

    GUIContainer builtIn = {};
    std::shared_ptr<GUIContainer> menuRoot = {};
    GameGuiBuilder builder;
};

