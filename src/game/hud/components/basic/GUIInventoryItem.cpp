//
// Created by aurailus on 2019-10-29.
//

#include "GUIInventoryItem.h"
#include "GUIText.h"
#include "GUIModel.h"

GUIInventoryItem::GUIInventoryItem(const std::string &key) : GUIContainer(key) {}

void GUIInventoryItem::create(glm::vec2 scale, unsigned short count, ItemDef& def, Font f) {

    if (def.type == ItemDef::Type::CRAFTITEM) {
        auto texture = static_cast<CraftItemDef&>(def).textureRefs[0];

        auto shadow = std::make_shared<GUIRect>("shadow");
        shadow->create(scale * 16.f, {}, texture, {0, 0, 0, 0.2});
        add(shadow);
        shadow->setPos(scale);

        auto item = std::make_shared<GUIRect>("item");
        item->create(scale * 16.f, {}, texture);
        add(item);
    }
    else {
        auto& model = static_cast<BlockDef&>(def).guiModel;

        auto item = std::make_shared<GUIModel>("item");
        item->create(scale * 10.5f, model);
        item->setPos(glm::vec2{7.75, 7.75} * scale);

        item->setDepth(25);
        glm::mat4 rot;
        rot = glm::rotate(rot, glm::radians(180.f), {1, 0, 0});
        rot = glm::rotate(rot, glm::radians( 45.f), {0, 1, 0});
        rot = glm::rotate(rot, glm::radians(-25.f), {1, 0, 1});
        item->setRotation(rot);

        add(item);
    }

    if (count > 1) {
        auto text = std::make_shared<GUIText>("count");
        text->create(scale, {}, {}, {1, 1, 1, 1}, f);
        text->setText(to_string(count));
        add(text);
        text->setPos({(19 - text->getWidth()) * scale.x, 9 * scale.y});
        text->setDepth(50);
    }
}
