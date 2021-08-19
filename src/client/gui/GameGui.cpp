////
//// Created by aurailus on 05/02/19.
////
//
//#include "GameGui.h"
//
//#include "../graph/Renderer.h"
//
//GameGui::GameGui(InventoryRefsPtr refs, Window& window, SubgamePtr defs, Renderer& renderer) :
//	refs(refs),
//	defs(defs),
//	window(window),
//	renderer(renderer),
//
//	hudBuilder(refs, window, defs, hudLuaRoot),
//	menuBuilder(refs, window, defs, menuLuaRoot) {
//
//	hudRoot->add(hudLuaRoot);
//	menuRoot->add(menuLuaRoot);
//
//	handList->create({ 3, 3 }, {}, {}, refs.l()->getCursorList(), refs.l()->getCursorList(), defs);
//	menuRoot->add(handList);
//}
//
//void GameGui::update(double delta) {
//	menuRoot->update(delta);
//	hudRoot->update(delta);
//
//	hudBuilder.update();
//	menuBuilder.update();
//
//	handList->setPos((renderer.window.input.getMousePos() - glm::ivec2(24)) / 3 * 3);
//}
//
//void GameGui::winResized(glm::ivec2 win) {
//	menuBuilder.build(win);
//	hudBuilder.build(win);
//}
//
//void GameGui::showMenu(std::shared_ptr<LuaGuiElement> root) {
//	menuBuilder.setGuiRoot(root);
//	menuBuilder.build(window.getSize());
//	inMenu = true;
//}
//
//void GameGui::closeMenu() {
//	menuBuilder.clear();
//	inMenu = false;
//}
//
//const bool GameGui::isInMenu() const {
//	return inMenu;
//}
//
//void GameGui::setHud(std::shared_ptr<LuaGuiElement> hud) {
//	this->hudRootElem = hud;
//	hudBuilder.setGuiRoot(hud);
//	hudBuilder.build(window.getSize());
//}
//
//std::shared_ptr<LuaGuiElement> GameGui::getHud() {
//	return hudRootElem;
//}
//
//void GameGui::setVisible(bool visible) {
//	menuRoot->setVisible(visible);
//	hudRoot->setVisible(visible);
//}
//
//bool GameGui::isVisible() {
//	return menuRoot->isVisible();
//}
//
//void GameGui::drawHud(Renderer& renderer) {
//	hudRoot->draw(renderer);
//}
//
//void GameGui::drawMenu(Renderer& renderer) {
//	menuRoot->draw(renderer);
//}