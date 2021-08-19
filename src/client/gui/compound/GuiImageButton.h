////
//// Created by aurailus on 2019-12-11.
////
//
//#pragma once
//
//#include "../basic/GuiRect.h"
//
//class GuiImageButton : public GuiRect {
//public:
//	using GuiRect::GuiRect;
//
//	static std::shared_ptr<GuiImageButton>
//	fromSerialized(const LuaGuiElement& elem, Window& window, TextureAtlas& textures, glm::ivec2 bounds);
//
//	void create(glm::vec2 scale, glm::vec4 padding, std::shared_ptr<AtlasRef> texture,
//		std::shared_ptr<AtlasRef> hoverTexture);
//
//	void setCallback(CallbackType type, const Callback& cb) override;
//
//	private:
//	void rebuild(bool hover);
//
//	std::shared_ptr<AtlasRef> hoverTexture = nullptr;
//};
