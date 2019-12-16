//
// Created by aurailus on 2019-10-29.
//

#include "GUIInventoryList.h"

GUIInventoryList::GUIInventoryList(const std::string &key) : GUIContainer(key) {}

void GUIInventoryList::create(glm::vec2 scale, glm::vec4 padding, glm::ivec2 innerPadding, InventoryList &list, InventoryList& hand, LocalDefs &defs) {
    this->list = &list;
    this->hand = &hand;
    this->defs = &defs;

    this->scale = scale;
    this->padding = padding;
    this->innerPadding = innerPadding;
    this->hitbox = glm::ivec2{
        padding.x + list.getWidth() * (innerPadding.x*scale.x),
        padding.y + (list.getLength() / list.getWidth()) * (innerPadding.y*scale.y)
    };

    drawContents();
    list.setUpdatedCallback(std::bind(&GUIInventoryList::drawContents, this));

    hoverRect->create({}, {}, {1, 1, 1, 0.1});

    setCallbacks(
            [=](bool down, glm::ivec2 pos) { this->leftClick(down, pos); },
            nullptr,
            [=](bool hovered, glm::ivec2 pos){this->hoverEvent(hovered, pos);});
}

void GUIInventoryList::setHoverCallback(const callback& hoverCallback) {
    GUIComponent::setHoverCallback([&, hoverCallback](bool hovered, glm::ivec2 pos) {
        if (hoverCallback) hoverCallback(hovered, pos);
        this->hoverEvent(hovered, pos);
    });
}

void GUIInventoryList::setLeftClickCallback(const callback& leftClickCallback) {
    GUIComponent::setLeftClickCallback([&, leftClickCallback](bool down, glm::ivec2 pos) {
        if (leftClickCallback) leftClickCallback(down, pos);
        this->leftClick(down, pos);
    });
}

void GUIInventoryList::setRightClickCallback(const callback& rightClickCallback) {
    GUIComponent::setRightClickCallback([&, rightClickCallback](bool down, glm::ivec2 pos) {
        if (rightClickCallback) rightClickCallback(down, pos);
        this->rightClick(down, pos);
    });
}

void GUIInventoryList::hoverEvent(bool hovered, glm::ivec2 pos) {
    pos += glm::ivec2(glm::vec2(this->padding.x, this->padding.y) * this->scale);

    if (hovered) {
        if (!this->hovered) hoverRect->setScale({16*scale.x, 16*scale.y});

        glm::ivec2 slot = pos / (glm::ivec2(this->scale) * this->innerPadding);
        slot.x = min(slot.x, static_cast<int>(list->getWidth() - 1));
        slot.y = min(slot.y, list->getLength() / list->getWidth() - 1);
        glm::ivec2 highlightPos = slot * glm::ivec2(this->scale) * this->innerPadding;

        hoverRect->setPos(highlightPos);
    }
    else if (this->hovered) hoverRect->setScale({});
}

void GUIInventoryList::leftClick(bool down, glm::ivec2 pos) {
    pos += glm::ivec2(glm::vec2(this->padding.x, this->padding.y) * this->scale);

    glm::ivec2 slot = pos / (glm::ivec2(this->scale) * this->innerPadding);
    slot.x = min(slot.x, static_cast<int>(list->getWidth() - 1));
    slot.y = min(slot.y, list->getLength() / list->getWidth() - 1);

    unsigned short index = slot.x + slot.y * list->getWidth();

    if (index < 0 || index >= list->getLength()) return;

    if (down) {
        hand->setStack(0, list->placeStack(index, hand->getStack(0)));
    }

//    leftClickIndex = index;
}

void GUIInventoryList::rightClick(bool down, glm::ivec2 pos) {
    pos += glm::ivec2(glm::vec2(this->padding.x, this->padding.y) * this->scale);

    glm::ivec2 slot = pos / (glm::ivec2(this->scale) * this->innerPadding);
    slot.x = min(slot.x, static_cast<int>(list->getWidth() - 1));
    slot.y = min(slot.y, list->getLength() / list->getWidth() - 1);

    unsigned short index = slot.x + slot.y * list->getWidth();

    if (index < 0 || index >= list->getLength()) return;

    if (down) {
        auto handStack = hand->getStack(0);
        if (handStack.count <= 0) {
            hand->setStack(0, list->splitStack(index));
        }
        else {
            auto handStack = hand->getStack(0);
            auto listStack = list->getStack(index);
            if (listStack.id == 0 || listStack.id == handStack.id) {
                auto overflow = list->placeStack(index, {handStack.id, 1});
                handStack.count -= 1;
                if (handStack.count == 0) handStack.id = 0;
                if (overflow.count != 0) handStack.count += overflow.count;
                hand->setStack(0, handStack);
            }
            else {
                hand->setStack(0, list->placeStack(index, hand->getStack(0)));
            }
        }
    }

//    leftClickIndex = index;
}

void GUIInventoryList::drawContents() {
    empty();

    auto fontRef = defs->textures["font"];
    Font f(defs->textures, fontRef);

    for (unsigned short i = 0; i < list->getLength() / list->getWidth(); i++) {
        for (unsigned short j = 0; j < list->getWidth(); j++) {
//            auto bg = std::make_shared<GUIRect>("background_" + to_string(i) + "_" + to_string(j));
//            bg->create(scale * 16.f, {}, {1, 0, 0, 0.3});
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

    add(hoverRect);
}

GUIInventoryList::~GUIInventoryList() {
    list->clearUpdatedCallback();
}