//
// Created by aurailus on 10/02/19.
//

#pragma once


#include <memory>

#include "../GuiComponent.h"
#include "../../SerialGui.h"
#include "../../../../def/texture/AtlasRef.h"

class ClientGame;

class GuiRect : public GuiComponent {
public:
    GuiRect() = default;
    GuiRect(const std::string& key);

    static std::shared_ptr<GuiRect> fromSerialized(const SerialGui::Element& elem, TextureAtlas& textures, glm::ivec2 bounds);

    void create(glm::vec2 scale, glm::vec4 padding, glm::vec4 color);
    void create(glm::vec2 scale, glm::vec4 padding, glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br);
    void create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture);
    void create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture, glm::vec4 tint);

protected:
    std::shared_ptr<AtlasRef> texture = nullptr;
};

