//
// Created by aurailus on 25/12/18.
//

#ifndef ZEUS_TEXTENTITY_H
#define ZEUS_TEXTENTITY_H


#include "../GUIComponent.h"
#include "../../../../../def/texture/AtlasRef.h"
#include "../../../../../def/texture/Font.h"
#include <utility>

class GUIText : public GUIComponent {
public:
    GUIText() = default;
    explicit GUIText(const std::string& key);

    void create(glm::vec2 scale, glm::vec4 padding, glm::vec4 bgcolor, glm::vec4 color, Font font);

    void setText(std::string text);
    std::string getText();
private:
    Font font;
    glm::vec4 bgcolor {};
    glm::vec4 color {};
    std::string text;
};


#endif //ZEUS_TEXTENTITY_H
