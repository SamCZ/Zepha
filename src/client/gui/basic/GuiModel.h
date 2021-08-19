////
//// Created by aurailus on 2019-12-17.
////
//
//#pragma once
//
//#include <string>
//
//#include "GuiContainer.h"
//
//class LocalSubgame;
//
//class ModelStore;
//
//class TextureAtlas;
//
//class LuaGuiElement;
//
//class GuiModel : public Element {
//public:
//	using Element::Element;
//
//	static std::shared_ptr<GuiModel>
//	fromSerialized(const LuaGuiElement& elem, Window& window, TextureAtlas& textures, ModelStore& models, glm::ivec2 bounds);
//
//	void create(glm::vec2 scale, std::shared_ptr<Model> model);
//
//	void update(double delta) override;
//
//	void animate(glm::vec2 range);
//
//	void setRotationX(float x);
//
//	void setRotationY(float x);
//
//	void setRotationZ(float x);
//
//	void draw(Renderer& renderer) override;
//
//	protected:
//	float depth = 300;
//};
