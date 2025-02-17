////
//// Created by aurailus on 10/02/19.
////
//
//#pragma once
//
//#include "client/gui/Element.h"
//
//class AtlasRef;
//class LocalSubgame;
//class TextureAtlas;
//class LuaGuiElement;
//
//class GuiRect : public Element {
//public:
//	using Element::Element;
//
//	static std::shared_ptr<GuiRect>
//	fromSerialized(const LuaGuiElement& elem, Window& window, TextureAtlas& textures, glm::ivec2 bounds);
//
//	void create(glm::vec2 scale, glm::vec4 padding, glm::vec4 color);
//
//	void create(glm::vec2 scale, glm::vec4 padding, glm::vec4 tl, glm::vec4 tr, glm::vec4 bl, glm::vec4 br);
//
//	void create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture);
//
//	void create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture, glm::vec4 tint);
//
//protected:
//	std::shared_ptr<AtlasRef> texture = nullptr;
//};
//
