//
// Created by aurailus on 25/12/18.
//

#pragma once

#include "client/gui/GuiComponent.h"

#include "client/graph/Font.h"

class TextureAtlas;

class LuaGuiElement;

class GuiText : public GuiComponent {
	public:
	GuiText() = default;
	
	explicit GuiText(const std::string& key);
	
	static std::shared_ptr<GuiText>
	fromSerialized(const LuaGuiElement& elem, TextureAtlas& textures, glm::ivec2 bounds);
	
	void create(glm::vec2 scale, glm::vec4 padding, glm::vec4 bgcolor, glm::vec4 color, Font font);
	
	unsigned int getWidth();
	
	void setText(std::string text);
	
	std::string getText();
	
	private:
	Font font;
	glm::vec4 bgcolor{};
	glm::vec4 color{};
	std::string text;
	
	unsigned int maxLineWidth = 0;
};

