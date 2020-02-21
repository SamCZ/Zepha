//
// Created by aurailus on 2019-12-12.
//

#include "GameGuiBuilder.h"

#include "components/compound/GuiInventoryList.h"

std::shared_ptr<GuiComponent> GameGuiBuilder::createComponent(SerialGui::Elem &data, glm::ivec2 bounds) {
    auto c = GuiBuilder::createComponent(data, bounds);
    if (c != nullptr) return c;

    GuiComponent::callback cbLeftClick = nullptr;
    GuiComponent::callback cbRightClick = nullptr;
    GuiComponent::callback cbHover = nullptr;

    if (callbacks.count(data.key)) {
        cbLeftClick = callbacks[data.key].left;
        cbRightClick = callbacks[data.key].right;
        cbHover = callbacks[data.key].hover;
    }

    switch (Util::hash(data.type.c_str())) {
        default: break;
        case Util::hash("inventory"): {
            c = GuiInventoryList::fromSerialized(data, game, bounds, inventory, hand);
            break;
        }
    }

    if (c != nullptr) c->setCallbacks(cbLeftClick, cbRightClick, cbHover);
    return c;
}
