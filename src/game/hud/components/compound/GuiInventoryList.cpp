//
// Created by aurailus on 2019-10-29.
//

#include "GuiInventoryList.h"

#include "../basic/GuiInventoryItem.h"
#include "../../../../def/texture/Font.h"
#include "../../../inventory/LocalInventoryList.cpp"

GuiInventoryList::GuiInventoryList(const std::string &key) : GuiContainer(key) {}

std::shared_ptr<GuiInventoryList> GuiInventoryList::fromSerialized(const LuaGuiElement& elem, ClientGame &game,
        glm::ivec2 bounds, LocalInventoryRefs& refs) {

    glm::vec2 pos     = SerialGui::get<glm::vec2>(elem, "position", bounds);
//    glm::vec2 offset  = SerialGui::get<glm::vec2>(elem, "position_anchor");
//    glm::vec2 size    = SerialGui::deserializeToken<glm::vec2>(s.tokens, "size", bounds);
    glm::vec4 padding = SerialGui::get<glm::vec4>(elem, "padding", bounds);
    glm::vec2 slotspc = SerialGui::get<glm::vec2>(elem, "slot_spacing", bounds);

    std::string source    = elem.get_or<std::string>("source", "");
    std::string list      = elem.get_or<std::string>("list", "");
    unsigned short start  = static_cast<unsigned short>(elem.get_or<float>("start", 1) - 1);
    unsigned short length = static_cast<unsigned short>(elem.get_or<float>("length", 0));

    auto invList = refs.getList(source, list);
    auto inv = std::make_shared<GuiInventoryList>(elem.key);

    inv->create(glm::vec2(SerialGui::SCALE_MODIFIER), padding * SerialGui::SCALE_MODIFIER,
            slotspc * SerialGui::SCALE_MODIFIER, invList, refs.getHand(), game, start, length);
    inv->setPos(pos);

    return inv;
}

void GuiInventoryList::create(glm::vec2 scale, glm::vec4 padding, glm::ivec2 innerPadding,
    std::shared_ptr<LocalInventoryList> list, std::shared_ptr<LocalInventoryList> hand, ClientGame& defs,
    unsigned short start, unsigned short length) {

    this->list = list;
    this->hand = hand;
    this->defs = &defs;

    this->start = start;
    this->length = length;

    this->scale = scale;
    this->padding = padding;
    this->innerPadding = innerPadding;
    this->hitbox = (list->getWidth() == 0 ? glm::ivec2 {} : glm::ivec2 {
        padding.x + list->getWidth() * (innerPadding.x*scale.x),
        padding.y + (list->getLength() / list->getWidth()) * (innerPadding.y*scale.y) });

    drawContents();
    myCallback = std::make_shared<std::function<void()>>(std::bind(&GuiInventoryList::drawContents, this));
    list->addGuiCallback(myCallback);

    setCallback(CallbackType::PRIMARY, nullptr);
    setCallback(CallbackType::SECONDARY, nullptr);
    setCallback(CallbackType::HOVER, nullptr);

    hoverRect->create({}, {}, {1, 1, 1, 0.1});
}

void GuiInventoryList::setCallback(CallbackType type, const callback& cb) {
    GuiComponent::setCallback(type, [&, cb, type](bool down, glm::ivec2 pos) {
        if (cb) cb(down, pos);

        if      (type == GuiComponent::CallbackType::PRIMARY  ) this->leftClick (down, pos);
        else if (type == GuiComponent::CallbackType::SECONDARY) this->rightClick(down, pos);
        else if (type == GuiComponent::CallbackType::HOVER    ) this->hoverEvent(down, pos);
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
    if (!down || list->getWidth() == 0) return;

    pos += glm::ivec2(glm::vec2(this->padding.x, this->padding.y) * this->scale);

    glm::ivec2 slot = pos / (glm::ivec2(this->scale) * this->innerPadding);
    slot.x = std::min(slot.x, static_cast<int>(list->getWidth() - 1));
    slot.y = std::min(slot.y, list->getLength() / list->getWidth() - 1);

    unsigned short index = slot.x + slot.y * list->getWidth();

    if (index >= list->getLength()) return;

    list->primaryInteract(*hand, index);
}

void GuiInventoryList::rightClick(bool down, glm::ivec2 pos) {
    if (!down || list->getWidth() == 0) return;

    pos += glm::ivec2(glm::vec2(this->padding.x, this->padding.y) * this->scale);

    glm::ivec2 slot = pos / (glm::ivec2(this->scale) * this->innerPadding);
    slot.x = std::min(slot.x, static_cast<int>(list->getWidth() - 1));
    slot.y = std::min(slot.y, list->getLength() / list->getWidth() - 1);

    unsigned short index = slot.x + slot.y * list->getWidth();
    if (index >= list->getLength()) return;

    list->secondaryInteract(*hand, index);
}

void GuiInventoryList::drawContents() {
    if (list->getWidth() == 0) return;
    unsigned short length = this->length == 0 ? list->getLength() : this->length;

    this->hitbox = glm::ivec2 {
        padding.x + list->getWidth() * (innerPadding.x*scale.x),
        padding.y + (list->getLength() / list->getWidth()) * (innerPadding.y*scale.y)
    };

    empty();

    auto fontRef = defs->textures["font"];
    Font f(defs->textures, fontRef);

    for (unsigned short i = 0; i < list->getLength() / list->getWidth(); i++) {
        for (unsigned short j = 0; j < list->getWidth(); j++) {
            unsigned short stackInd = j + i * list->getWidth();
            if (stackInd >= length) break;
//            auto bg = std::make_shared<GuiRect>("background_" + to_string(i) + "_" + to_string(j));
//            bg->create(scale * 16.f, {}, {1, 0, 0, 0.3});
//            add(bg);
//            bg->setPos({padding.x + j * (16*scale.x+innerPadding.x/scale.x), padding.y + i * (16*scale.y+innerPadding.y/scale.y)});

            auto stack = list->getStack(stackInd);
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
    if (list != nullptr) list->removeGuiCallback(myCallback);
}