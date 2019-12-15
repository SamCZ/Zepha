//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include "../basic/GUIContainer.h"
#include "../../../../def/LocalDefs.h"
#include "../basic/GUIRect.h"
#include "../basic/GUIInventoryItem.h"
#include "../../../../def/texture/Font.h"
#include "../../../scene/world/InventoryList.h"

class GUIInventoryList : public GUIContainer {
public:
    GUIInventoryList() = default;
    GUIInventoryList(const std::string& key);
    ~GUIInventoryList() override;

    void create(glm::vec2 scale, glm::vec4 padding, glm::ivec2 innerPadding, InventoryList& list, InventoryList& hand, LocalDefs& defs);

    void setHoverCallback(std::function<void(bool, glm::ivec2)> hoverCallback) override;
    void setClickCallback(std::function<void(glm::ivec2)> clickCallback) override;

    void hoverEvent(bool hovered, glm::ivec2 pos);
    void clickEvent(glm::ivec2 pos);
    void drawContents();
private:

    std::shared_ptr<GUIRect> hoverRect = std::make_shared<GUIRect>("hover_rect");

    InventoryList* list;
    InventoryList* hand;
    LocalDefs* defs;
    glm::ivec2 innerPadding;
};
