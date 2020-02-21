//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include "../basic/GUIContainer.h"

#include "../basic/GUIRect.h"
#include "../../SerialGui.h"
#include "../../../../def/ClientGame.h"
#include "../../../scene/world/Inventory.h"
#include "../../../scene/world/InventoryList.h"

class GUIInventoryList : public GUIContainer {
public:
    GUIInventoryList() = default;
    GUIInventoryList(const std::string& key);
    ~GUIInventoryList() override;

    static std::shared_ptr<GUIInventoryList> fromSerialized(SerialGui::Elem s, ClientGame &game,
            glm::ivec2 bounds, Inventory& inventory, InventoryList& hand);

    void create(glm::vec2 scale, glm::vec4 padding, glm::ivec2 innerPadding, InventoryList& list, InventoryList& hand, ClientGame& defs);

    void setHoverCallback(const callback& hoverCallback) override;
    void setLeftClickCallback(const callback& leftClickCallback) override;
    void setRightClickCallback(const callback& rightClickCallback) override;

    void hoverEvent(bool hovered, glm::ivec2 pos);
    void leftClick(bool down, glm::ivec2 pos);
    void rightClick(bool down, glm::ivec2 pos);
    void drawContents();
private:
    std::shared_ptr<GUIRect> hoverRect = std::make_shared<GUIRect>("hover_rect");

    InventoryList* list;
    InventoryList* hand;
    ClientGame* defs;
    glm::ivec2 innerPadding;
};
