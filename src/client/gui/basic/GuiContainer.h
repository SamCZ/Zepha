//
// Created by aurailus on 27/07/19.
//

#pragma once

#include "client/gui/GuiComponent.h"

class GuiContainer : public GuiComponent {
	public:
	GuiContainer() = default;
	
	explicit GuiContainer(const std::string& key);
	
	void draw(Renderer& renderer) override;
};

