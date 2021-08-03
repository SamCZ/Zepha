#pragma once

#include "client/gui/GameGuiBuilder.h"
#include "client/gui/compound/GuiInventoryList.h"

class GameGui {
public:
	explicit GameGui(InventoryRefsPtr refs, glm::vec2 bufferSize, SubgamePtr defs, Renderer& renderer);

	void winResized(glm::ivec2 win);

	void update(double delta);

	void setVisible(bool visible);

	bool isVisible();

	void showMenu(std::shared_ptr<LuaGuiElement> root);

	void closeMenu();

	const bool isInMenu() const;

	void setHud(std::shared_ptr<LuaGuiElement> hud);

	std::shared_ptr<LuaGuiElement> getHud();

	void drawHud(Renderer& renderer);

	void drawMenu(Renderer& renderer);

private:
	SubgamePtr defs;
	Renderer& renderer;

	ivec2 win {};
	bool inMenu = false;

	std::shared_ptr<LuaGuiElement> hudRootElem = nullptr;

	std::shared_ptr<GuiContainer> menuRoot = std::make_shared<GuiInventoryList>("menuRoot");
	std::shared_ptr<GuiContainer> menuLuaRoot = std::make_shared<GuiInventoryList>("menuLuaRoot");
	GameGuiBuilder menuBuilder;
	std::shared_ptr<GuiContainer> hudRoot = std::make_shared<GuiInventoryList>("hudRoot");
	std::shared_ptr<GuiContainer> hudLuaRoot = std::make_shared<GuiInventoryList>("hudLuaRoot");
	GameGuiBuilder hudBuilder;

	std::shared_ptr<GuiInventoryList> handList = std::make_shared<GuiInventoryList>("hand");

	InventoryRefsPtr refs;
};

