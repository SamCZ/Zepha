//
// Created by aurailus on 2019-10-29.
//

#include "GuiInventoryList.h"

#include "../basic/GuiInventoryItem.h"
#include "../../../inventory/LocalInventoryList.cpp"
#include "../../../../def/texture/Font.h"
GuiInventoryList::GuiInventoryList(const std::string &key) : GuiContainer(key) {}

std::shared_ptr<GuiInventoryList> GuiInventoryList::fromSerialized(SerialGui::Elem s, ClientGame &game,
        glm::ivec2 bounds, LocalInventoryRefs& refs) {

    glm::vec2 pos     = SerialGui::deserializeToken<glm::vec2>(s.tokens, "position", bounds);
    glm::vec2 offset  = SerialGui::deserializeToken<glm::vec2>(s.tokens, "position_anchor");
//    glm::vec2 size    = SerialGui::deserializeToken<glm::vec2>(s.tokens, "size", bounds);
    glm::vec4 padding = SerialGui::deserializeToken<glm::vec4>(s.tokens, "padding", bounds);
    glm::vec2 slotspc = SerialGui::deserializeToken<glm::vec2>(s.tokens, "slot_spacing", bounds);

    std::string source = s.tokens["source"];
    std::string list   = s.tokens["list"];

    auto invList = refs.getList(source, list);
    auto inv = std::make_shared<GuiInventoryList>(s.key);

    inv->create(glm::vec2(SerialGui::SCALE_MODIFIER), padding * SerialGui::SCALE_MODIFIER,
            slotspc * SerialGui::SCALE_MODIFIER, invList, refs.getHand(), game);
    inv->setPos(pos);

    return inv;
}

void GuiInventoryList::create(glm::vec2 scale, glm::vec4 padding, glm::ivec2 innerPadding,
        std::shared_ptr<LocalInventoryList> list, std::shared_ptr<LocalInventoryList> hand, ClientGame& defs) {
    this->list = list;
    this->hand = hand;
    this->defs = &defs;

    this->scale = scale;
    this->padding = padding;
    this->innerPadding = innerPadding;
    this->hitbox = glm::ivec2 {
        padding.x + list->getWidth() * (innerPadding.x*scale.x),
        padding.y + (list->getLength() / list->getWidth()) * (innerPadding.y*scale.y)
    };

    drawContents();
    list->setGuiCallback(std::bind(&GuiInventoryList::drawContents, this));

    hoverRect->create({}, {}, {1, 1, 1, 0.1});

    setCallbacks(
            [=](bool down, glm::ivec2 pos) { this->leftClick(down, pos); },
            nullptr,
            [=](bool hovered, glm::ivec2 pos){this->hoverEvent(hovered, pos);});
}

void GuiInventoryList::setHoverCallback(const callback& hoverCallback) {
    GuiComponent::setHoverCallback([&, hoverCallback](bool hovered, glm::ivec2 pos) {
        if (hoverCallback) hoverCallback(hovered, pos);
        this->hoverEvent(hovered, pos);
    });
}

void GuiInventoryList::setLeftClickCallback(const callback& leftClickCallback) {
    GuiComponent::setLeftClickCallback([&, leftClickCallback](bool down, glm::ivec2 pos) {
        if (leftClickCallback) leftClickCallback(down, pos);
        this->leftClick(down, pos);
    });
}

void GuiInventoryList::setRightClickCallback(const callback& rightClickCallback) {
    GuiComponent::setRightClickCallback([&, rightClickCallback](bool down, glm::ivec2 pos) {
        if (rightClickCallback) rightClickCallback(down, pos);
        this->rightClick(down, pos);
    });
}

void GuiInventoryList::hoverEvent(bool hovered, glm::ivec2 pos) {
    pos += glm::ivec2(glm::vec2(this->padding.x, this->padding.y) * this->scale);

    if (hovered) {
        if (!this->hovered) hoverRect->setScale({16*scale.x, 16*scale.y});

        glm::ivec2 slot = pos / (glm::ivec2(this->scale) * this->innerPadding);
        slot.x = std::min(slot.x, static_cast<int>(list->getWidth() - 1));
        slot.y = std::min(slot.y, list->getLength() / list->getWidth() - 1);
        glm::ivec2 highlightPos = slot * glm::ivec2(this->scale) * this->innerPadding;

        hoverRect->setPos(highlightPos);
    }
    else if (this->hovered) hoverRect->setScale({});
}

void GuiInventoryList::leftClick(bool down, glm::ivec2 pos) {
    if (!down) return;

    pos += glm::ivec2(glm::vec2(this->padding.x, this->padding.y) * this->scale);

    glm::ivec2 slot = pos / (glm::ivec2(this->scale) * this->innerPadding);
    slot.x = std::min(slot.x, static_cast<int>(list->getWidth() - 1));
    slot.y = std::min(slot.y, list->getLength() / list->getWidth() - 1);

    unsigned short index = slot.x + slot.y * list->getWidth();

    if (index < 0 || index >= list->getLength()) return;

    hand->setStack(0, list->placeStack(index, hand->getStack(0), true));
}

void GuiInventoryList::rightClick(bool down, glm::ivec2 pos) {
    pos += glm::ivec2(glm::vec2(this->padding.x, this->padding.y) * this->scale);

    glm::ivec2 slot = pos / (glm::ivec2(this->scale) * this->innerPadding);
    slot.x = std::min(slot.x, static_cast<int>(list->getWidth() - 1));
    slot.y = std::min(slot.y, list->getLength() / list->getWidth() - 1);

    unsigned short index = slot.x + slot.y * list->getWidth();
    if (index >= list->getLength()) return;

    if (down) {
        auto handStack = hand->getStack(0);
        if (handStack.count == 0) {
            hand->setStack(0, list->splitStack(index, true));
        }
        else {
            auto handStack = hand->getStack(0);
            auto listStack = list->getStack(index);
            if (listStack.id == 0 || listStack.id == handStack.id) {
                auto overflow = list->placeStack(index, {handStack.id, 1}, true);
                handStack.count -= 1;
                if (handStack.count == 0) handStack.id = 0;
                if (overflow.count != 0) handStack.count += overflow.count;
                hand->setStack(0, handStack);
            }
            else {
                hand->setStack(0, list->placeStack(index, hand->getStack(0), true));
            }
        }
    }
}

void GuiInventoryList::drawContents() {
    empty();

    auto fontRef = defs->textures["font"];
    Font f(defs->textures, fontRef);

    for (unsigned short i = 0; i < list->getLength() / list->getWidth(); i++) {
        for (unsigned short j = 0; j < list->getWidth(); j++) {
//            auto bg = std::make_shared<GuiRect>("background_" + to_string(i) + "_" + to_string(j));
//            bg->create(scale * 16.f, {}, {1, 0, 0, 0.3});
//            add(bg);
//            bg->setPos({padding.x + j * (16*scale.x+innerPadding.x/scale.x), padding.y + i * (16*scale.y+innerPadding.y/scale.y)});

            auto stack = list->getStack(j + i * list->getWidth());
            if (stack.id == 0) continue;

            auto item = std::make_shared<GuiInventoryItem>("item_" + std::to_string(i) + "_" + std::to_string(j));
            item->create(scale, stack.count, defs->defs.fromId(stack.id), f);
            add(item);
            item->setPos({padding.x + j * (16*scale.x+innerPadding.x/scale.x), padding.y + i * (16*scale.y+innerPadding.y/scale.y)});
        }
    }

    add(hoverRect);
}

GuiInventoryList::~GuiInventoryList() {
    list->setGuiCallback(nullptr);
}