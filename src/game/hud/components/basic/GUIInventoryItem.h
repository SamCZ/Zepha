//
// Created by aurailus on 2019-10-29.
//

#pragma once

#include "GUIRect.h"
#include "../../../../def/texture/Font.h"
#include "GUIContainer.h"

class GUIInventoryItem : public GUIContainer {
public:
    GUIInventoryItem() = default;
    GUIInventoryItem(const std::string& key);

    void create(glm::vec2 scale, unsigned short itemCount, std::shared_ptr<AtlasRef> texture, Font font);
};
