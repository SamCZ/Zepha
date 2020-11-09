//
// Created by aurailus on 27/07/19.
//

#include "GuiContainer.h"

GuiContainer::GuiContainer(const std::string& key) : GuiComponent(key) {}

void GuiContainer::draw(Renderer& renderer) {
	for (const auto& child : children) {
		child->draw(renderer);
	}
}
