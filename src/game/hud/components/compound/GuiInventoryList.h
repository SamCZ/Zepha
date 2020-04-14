//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include "../basic/GuiContainer.h"

#include "../basic/GuiRect.h"
#include "../../SerialGui.h"
#include "../../../../def/ClientGame.h"
#include "../../../inventory/Inventory.h"
#include "../../../inventory/LocalInventoryList.h"
#include "../../../inventory/LocalInventoryRefs.h"

class GuiInventoryList : public GuiContainer {
public:
    GuiInventoryList() = default;
    GuiInventoryList(const std::string& key);
    ~GuiInventoryList() override;

    static std::shared_ptr<GuiInventoryList> fromSerialized(const LuaGuiElement& elem, ClientGame &game,
            glm::ivec2 bounds, LocalInventoryRefs& refs);

    void create(glm::vec2 scale, glm::vec4 padding, glm::ivec2 innerPadding,
            std::shared_ptr<LocalInventoryList>  list, std::shared_ptr<LocalInventoryList>  hand, ClientGame& defs);

    void setCallback(CallbackType type, const callback& cb) override;

    void hoverEvent(bool hovered, glm::ivec2 pos);
    void leftClick(bool down, glm::ivec2 pos);
    void rightClick(bool down, glm::ivec2 pos);
    void drawContents();
private:
    std::shared_ptr<GuiRect> hoverRect = std::make_shared<GuiRect>("hover_rect");

    std::shared_ptr<LocalInventoryList> list, hand;
    glm::ivec2 innerPadding;
    ClientGame* defs = nullptr;
};
