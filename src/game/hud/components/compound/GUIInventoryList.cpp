//
// Created by aurailus on 2019-10-29.
//

#include "GUIInventoryList.h"

GUIInventoryList::GUIInventoryList(const std::string &key) : GUIContainer(key) {}

void GUIInventoryList::create(glm::vec2 scale, glm::vec4 padding, glm::vec2 innerPadding, InventoryList &list, LocalDefs &defs) {
    this->list = &list;
    this->defs = &defs;

    this->scale = scale;
    this->padding = padding;
    this->innerPadding = innerPadding;

    drawContents();
    list.setUpdatedCallback(std::bind(&GUIInventoryList::drawContents, this));
}

void GUIInventoryList::drawContents() {
    empty();

    auto fontRef = defs->textures["font"];
    Font f(defs->textures, fontRef);

    for (unsigned short i = 0; i < list->getLength() / list->getWidth(); i++) {
        for (unsigned short j = 0; j < list->getWidth(); j++) {
//            auto bg = std::make_shared<GUIRect>("background_" + to_string(i) + "_" + to_string(j));
//            bg->create(scale * 16.f, {}, {1, 0, 0, 1});
//            add(bg);
//            bg->setPos({padding.x + j * (16*scale.x+innerPadding.x/scale.x), padding.y + i * (16*scale.y+innerPadding.y/scale.y)});

            auto stack = list->getStack(j + i * list->getWidth());
            if (stack.id == 0) continue;

            auto item = std::make_shared<GUIInventoryItem>("item_" + to_string(i) + "_" + to_string(j));
            item->create(scale, stack.count, defs->textures[defs->defs.craftItemFromId(stack.id).textures[0]], f);
            add(item);
            item->setPos({padding.x + j * (16*scale.x+innerPadding.x/scale.x), padding.y + i * (16*scale.y+innerPadding.y/scale.y)});
        }
    }
}

GUIInventoryList::~GUIInventoryList() {
    list->clearUpdatedCallback();
}