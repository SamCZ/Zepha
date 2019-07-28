//
// Created by aurailus on 27/07/19.
//

#include "GUIContainer.h"

void GUIContainer::draw(Renderer &renderer) {
    for (const auto& child : children) {
        child.second->draw(renderer);
    }
}
