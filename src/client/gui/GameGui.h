//#pragma once
//
//#include "client/graph/Renderer.h"
//#include "client/gui/GameGuiBuilder.h"
//#include "client/gui/compound/GuiInventoryList.h"
//
//class GameGui {
//public:
//	explicit GameGui(InventoryRefsPtr refs, Window& window, SubgamePtr defs, Renderer& renderer);
//
//	void winResized(glm::ivec2 win);
//
//	void update(double delta);
//
//	void setVisible(bool visible);
//
//	bool isVisible();
//
//	void showMenu(std::shared_ptr<LuaGuiElement> root);
//
//	void closeMenu();
//
//	const bool isInMenu() const;
//
//	void setHud(std::shared_ptr<LuaGuiElement> hud);
//
//	std::shared_ptr<LuaGuiElement> getHud();
//
//	void drawHud(Renderer& renderer);
//
//	void drawMenu(Renderer& renderer);
//
//private:
//	SubgamePtr defs;
//	Renderer& renderer;
//
//	Window& window;
//	bool inMenu = false;
//
//	std::shared_ptr<LuaGuiElement> hudRootElem = nullptr;
//
//	std::shared_ptr<GuiContainer> menuRoot = std::make_shared<GuiInventoryList>(renderer.window, "menuRoot");
//	std::shared_ptr<GuiContainer> menuLuaRoot = std::make_shared<GuiInventoryList>(renderer.window, "menuLuaRoot");
//	GameGuiBuilder menuBuilder;
//	std::shared_ptr<GuiContainer> hudRoot = std::make_shared<GuiInventoryList>(renderer.window, "hudRoot");
//	std::shared_ptr<GuiContainer> hudLuaRoot = std::make_shared<GuiInventoryList>(renderer.window, "hudLuaRoot");
//	GameGuiBuilder hudBuilder;
//
//	std::shared_ptr<GuiInventoryList> handList = std::make_shared<GuiInventoryList>(renderer.window, "hand");
//
//	InventoryRefsPtr refs;
//};
//
