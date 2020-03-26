//
// Created by aurailus on 2019-12-12.
//

#include <array>

#include "GameGuiBuilder.h"

#include "components/compound/GuiInventoryList.h"

std::shared_ptr<GuiComponent> GameGuiBuilder::createComponent(const SerialGui::Element& elem, glm::ivec2 bounds) {
    auto c = GuiBuilder::createComponent(elem, bounds);
    if (c != nullptr) return c;

    switch (Util::hash(elem.type.c_str())) {
        default: break;
        case Util::hash("inventory_list"): {
            c = GuiInventoryList::fromSerialized(elem, defs, bounds, refs);
            break;
        }
    }

    if (!c) return nullptr;

    if (elem.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::PRIMARY)])
        c->setCallback(GuiComponent::CallbackType::PRIMARY, elem.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::PRIMARY)]);

    if (elem.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::SECONDARY)])
        c->setCallback(GuiComponent::CallbackType::SECONDARY, elem.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::SECONDARY)]);

    if (elem.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::HOVER)])
        c->setCallback(GuiComponent::CallbackType::HOVER, elem.callbacks[static_cast<unsigned int>(GuiComponent::CallbackType::HOVER)]);

    return c;
}
