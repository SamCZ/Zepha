//
// Created by aurailus on 2019-12-12.
//

#include "GameGuiBuilder.h"

#include "components/compound/GUIInventoryList.h"

std::shared_ptr<GUIComponent> GameGuiBuilder::createComponent(SerialGui::Elem &data, glm::ivec2 bounds) {
    auto c = GuiBuilder::createComponent(data, bounds);
    if (c != nullptr) return c;

    GUIComponent::callback cbLeftClick = nullptr;
    GUIComponent::callback cbRightClick = nullptr;
    GUIComponent::callback cbHover = nullptr;

    if (callbacks.count(data.key)) {
        cbLeftClick = callbacks[data.key].left;
        cbRightClick = callbacks[data.key].right;
        cbHover = callbacks[data.key].hover;
    }

    switch (Util::hash(data.type.c_str())) {
        default: break;
        case Util::hash("inventory"): {
            c = GUIInventoryList::fromSerialized(data, game, bounds, inventory, hand);
            break;
        }
    }

    if (c != nullptr) c->setCallbacks(cbLeftClick, cbRightClick, cbHover);
    return c;
}
