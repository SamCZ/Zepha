//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include "GuiBuilder.h"
#include "../scene/world/InventoryList.h"

class GameGuiBuilder : public GuiBuilder {
public:
    GameGuiBuilder(InventoryList& list, LocalDefs& defs, std::shared_ptr<GUIContainer> root) : list(list), GuiBuilder(defs, root) {};
    std::shared_ptr<GUIComponent> createComponent(SerializedGuiElem& data) override;

private:
    InventoryList& list;
};
