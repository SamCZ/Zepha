////
//// Created by aurailus on 2019-10-29.
////
//
//#pragma once
//
//#include "../basic/GuiRect.h"
//#include "../basic/GuiContainer.h"
//#include "util/CovariantPtr.h"
//
//class LocalSubgame;
//class LuaGuiElement;
//class LocalInventoryRefs;
//class LocalInventoryList;
//
//class GuiInventoryList : public GuiContainer {
//public:
//	using GuiContainer::GuiContainer;
//
//	~GuiInventoryList() override;
//
//	static std::shared_ptr<GuiInventoryList> fromSerialized(const LuaGuiElement& elem,
//		Window& window, SubgamePtr game, glm::ivec2 bounds, InventoryRefsPtr refs);
//
//	void create(glm::vec2 scale, glm::vec4 padding, glm::ivec2 innerPadding,
//		InventoryListPtr list, InventoryListPtr cursor, SubgamePtr defs,
//		unsigned short start = 0, unsigned short length = 0);
//
//	void setCallback(CallbackType type, const Callback& cb) override;
//
//	void hoverEvent(bool hovered, glm::ivec2 pos);
//
//	void interactEvent(glm::ivec2 pos, bool primary);
//
//	void drawContents();
//
//private:
//	std::shared_ptr<GuiRect> hoverRect = std::make_shared<GuiRect>(window, "hover_rect");
//	std::shared_ptr<std::function<void()>> myCallback = nullptr;
//
//	InventoryListPtr list;
//	InventoryListPtr cursor;
//
//	unsigned short start = 0;
//	unsigned short length = 0;
//
//	glm::ivec2 innerPadding;
//
//	SubgamePtr defs = SubgamePtr();
//};
