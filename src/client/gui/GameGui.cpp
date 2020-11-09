//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"

#include "../graph/Renderer.h"

GameGui::GameGui(InventoryRefsPtr refs, glm::vec2 bufferSize, SubgamePtr defs, Renderer& renderer) :
	refs(refs),
	defs(defs),
	win(bufferSize),
	renderer(renderer),
	
	hudBuilder(refs, defs, hudLuaRoot),
	menuBuilder(refs, defs, menuLuaRoot) {
	
	hudRoot->add(hudLuaRoot);
	menuRoot->add(menuLuaRoot);
	
	handList->create({ 3, 3 }, {}, {}, refs.l()->getCursorList(), refs.l()->getCursorList(), defs);
	menuRoot->add(handList);
}

void GameGui::update(double delta) {
	menuRoot->update(delta);
	hudRoot->update(delta);
	
	hudBuilder.update();
	menuBuilder.update();
	
	handList->setPos((renderer.window.input.mousePos() - glm::ivec2(24)) / 3 * 3);
	menuRoot->handleMouseInput(renderer.window);
}

void GameGui::winResized(glm::ivec2 win) {
	this->win = win;
	menuBuilder.build(win);
	hudBuilder.build(win);
}

void GameGui::showMenu(std::shared_ptr<LuaGuiElement> root) {
	menuBuilder.setGuiRoot(root);
	menuBuilder.build(win);
	inMenu = true;
}

void GameGui::closeMenu() {
	menuBuilder.clear();
	inMenu = false;
}

const bool GameGui::isInMenu() const {
	return inMenu;
}

void GameGui::setHud(std::shared_ptr<LuaGuiElement> hud) {
	this->hudRootElem = hud;
	hudBuilder.setGuiRoot(hud);
	hudBuilder.build(win);
}

std::shared_ptr<LuaGuiElement> GameGui::getHud() {
	return hudRootElem;
}

void GameGui::setVisible(bool visible) {
	menuRoot->setVisible(visible);
	hudRoot->setVisible(visible);
}

bool GameGui::isVisible() {
	return menuRoot->isVisible();
}

void GameGui::drawHud(Renderer& renderer) {
	hudRoot->draw(renderer);
}

void GameGui::drawMenu(Renderer& renderer) {
	menuRoot->draw(renderer);
}