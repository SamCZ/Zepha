//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include "GuiBuilder.h"
#include "../scene/world/InventoryList.h"
#include "../scene/world/Inventory.h"

class GameGuiBuilder : public GuiBuilder {
public:
    GameGuiBuilder(Inventory& inventory, InventoryList& hand, ClientGame& defs, std::shared_ptr<GuiContainer> root) :
        inventory(inventory), hand(hand), GuiBuilder(defs, root) {};

    std::shared_ptr<GuiComponent> createComponent(SerialGui::Elem& data, glm::ivec2 bounds) override;

private:
    Inventory& inventory;
    InventoryList& hand;
};
