//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include "GuiContainer.h"

class ItemDef;
class Font;

class GuiInventoryItem : public GuiContainer {
public:
    GuiInventoryItem() = default;
    GuiInventoryItem(const std::string& key);

    void create(glm::vec2 scale, unsigned short itemCount, ItemDef& def, Font font);
};
