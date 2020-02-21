//
// Created by aurailus on 25/12/18.
//

#pragma once

#include <utility>

#include "../GuiComponent.h"

#include "../../SerialGui.h"
#include "../../../../def/texture/Font.h"
#include "../../../../def/texture/AtlasRef.h"

class GuiText : public GuiComponent {
public:
    GuiText() = default;
    explicit GuiText(const std::string& key);

    static std::shared_ptr<GuiText> fromSerialized(SerialGui::Elem s, ClientGame& game, glm::ivec2 bounds);

    void create(glm::vec2 scale, glm::vec4 padding, glm::vec4 bgcolor, glm::vec4 color, Font font);

    unsigned int getWidth();

    void setText(std::string text);
    std::string getText();
private:
    Font font;
    glm::vec4 bgcolor {};
    glm::vec4 color {};
    std::string text;

    unsigned int maxLineWidth = 0;
};

