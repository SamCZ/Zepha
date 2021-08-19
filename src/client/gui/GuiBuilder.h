////
//// Created by aurailus on 2019-12-12.
////
//
//#pragma once
//
//#include "SerialGui.h"
//#include "Element.h"
//#include "game/LocalSubgame.h"
//#include "../../lua/usertype/LuaGuiElement.h"
//
//class GuiContainer;
//
//class GuiBuilder {
//public:
//	struct ComponentCallbacks {
//		Element::Callback left {}, right {}, hover {};
//	};
//
//	GuiBuilder(TextureAtlas& textures, Window& window, ModelStore& models, std::shared_ptr<GuiContainer> root);
//
//	void setGuiRoot(std::shared_ptr<LuaGuiElement> menu);
//
//	void update();
//
//	void build(glm::ivec2 winBounds = {});
//
//	void clear(bool deleteRoot = true);
//
//	~GuiBuilder();
//
//	protected:
//	void create(LuaGuiElement& element, std::shared_ptr<Element> parent, glm::ivec2 bounds);
//
//	virtual std::shared_ptr<Element> createComponent(LuaGuiElement& elem, glm::ivec2 bounds);
//
//	static void clearCallbacks(std::shared_ptr<Element> component);
//
//	void elementUpdated();
//
//	TextureAtlas& textures;
//	ModelStore& models;
//	Window& window;
//
//	std::shared_ptr<GuiContainer> root = nullptr;
//
//	std::shared_ptr<LuaGuiElement> elements = nullptr;
//	unsigned int keyInd = 0;
//
//	bool dirty = false;
//
//	glm::ivec2 winBounds{};
//};
