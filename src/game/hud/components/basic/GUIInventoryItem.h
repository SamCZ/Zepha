//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include "GUIContainer.h"

class ItemDef;
class Font;

class GUIInventoryItem : public GUIContainer {
public:
    GUIInventoryItem() = default;
    GUIInventoryItem(const std::string& key);

    void create(glm::vec2 scale, unsigned short itemCount, ItemDef& def, Font font);
};
