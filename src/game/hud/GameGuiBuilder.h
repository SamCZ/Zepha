//
// Created by aurailus on 2019-12-12.
//

#pragma once

#include "GuiBuilder.h"

#include "../inventory/ServerInventory.h"
#include "../inventory/LocalInventoryRefs.h"

class GameGuiBuilder : public GuiBuilder {
public:
    GameGuiBuilder(LocalInventoryRefs& refs, LocalSubgame& defs, std::shared_ptr<GuiContainer> root) :
        defs(defs), refs(refs), GuiBuilder(defs.textures, defs.models, root) {};

    std::shared_ptr<GuiComponent> createComponent(LuaGuiElement& elem, glm::ivec2 bounds) override;
private:
    LocalInventoryRefs& refs;
    LocalSubgame& defs;
};
