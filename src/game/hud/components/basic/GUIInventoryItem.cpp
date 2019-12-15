//
// Created by aurailus on 2019-10-29.
//

#include "GUIInventoryItem.h"
#include "GUIText.h"

GUIInventoryItem::GUIInventoryItem(const std::string &key) : GUIContainer(key) {}

void GUIInventoryItem::create(glm::vec2 scale, unsigned short count, std::shared_ptr<AtlasRef> texture, Font f) {

    auto shadow = std::make_shared<GUIRect>("shadow");
    shadow->create(scale * 16.f, {}, texture, {0, 0, 0, 0.2});
    add(shadow);
    shadow->setPos(scale);

    auto item = std::make_shared<GUIRect>("item");
    item->create(scale * 16.f, {}, texture);
    add(item);

    if (count > 1) {
        auto text = std::make_shared<GUIText>("count");
        text->create(scale, {}, {}, {1, 1, 1, 1}, f);
        text->setText(to_string(count));
        add(text);
        text->setPos({(19 - text->getWidth()) * scale.x, 9 * scale.y});
    }
}
