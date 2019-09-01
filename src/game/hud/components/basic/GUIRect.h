//
// Created by aurailus on 10/02/19.
//

#pragma once


#include <memory>
#include "../GUIComponent.h"
#include "../../../../def/texture/AtlasRef.h"

class GUIRect : public GUIComponent {
public:
    GUIRect() = default;
    GUIRect(const std::string& key);

    void create(glm::vec2 scale, glm::vec4 padding, glm::vec4 color);
    void create(glm::vec2 scale, glm::vec4 padding, glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br);
    void create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture);
private:
    std::shared_ptr<AtlasRef> texture = nullptr;
};

