//
// Created by aurailus on 2019-10-29.
//

#include "GUIInventoryList.h"

GUIInventoryList::GUIInventoryList(const std::string &key) : GUIContainer(key) {}

void GUIInventoryList::create(glm::vec2 scale, glm::vec4 padding, glm::vec2 innerPadding,
        unsigned int listWidth, unsigned int listHeight, LocalDefs &defs) {

    this->scale = scale;
    this->padding = padding;

    auto fontRef = defs.textures().getTextureRef("font");
    Font f(defs.textures(), fontRef);

    empty();

    std::string data[4] = {
            "zeus:materials:stick_0",
            "zeus:materials:stick_2",
            "zeus:materials:rock",
            "zeus:materials:flint",
    };

    for (unsigned short i = 0; i < listHeight; i++) {
        for (unsigned short j = 0; j < listWidth; j++) {
            auto bg = std::make_shared<GUIRect>("background_" + to_string(i) + "_" + to_string(j));
            bg->create(scale * 16.f, {}, {.6, .6, .6, .3});
            add(bg);
            bg->setPos({padding.x + j * (16+innerPadding.x)*scale.x, padding.y + i * (16+innerPadding.y)*scale.y});

            auto item = std::make_shared<GUIInventoryItem>("item_" + to_string(i) + "_" + to_string(j));
            item->create(scale, random() % 4, defs.textures().getTextureRef(data[static_cast<int>(random() % 4)]), f);
            add(item);
            item->setPos({padding.x + j * (16+innerPadding.x)*scale.x, padding.y + i * (16+innerPadding.y)*scale.y});
        }
    }
}
