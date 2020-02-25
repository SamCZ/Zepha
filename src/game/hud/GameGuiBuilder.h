//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include "GuiBuilder.h"

#include "../scene/world/Inventory.h"
#include "../scene/world/LocalInventoryRefs.h"

class GameGuiBuilder : public GuiBuilder {
public:
    GameGuiBuilder(LocalInventoryRefs& refs, ClientGame& defs, std::shared_ptr<GuiContainer> root) :
        refs(refs), GuiBuilder(defs, root) {};

    std::shared_ptr<GuiComponent> createComponent(SerialGui::Elem& data, glm::ivec2 bounds) override;
private:
    LocalInventoryRefs& refs;
};
