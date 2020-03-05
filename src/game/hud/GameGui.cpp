//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"
#include "components/compound/GuiInventoryList.h"

GameGui::GameGui(LocalInventoryRefs& refs, glm::vec2 bufferSize, ClientGame& defs, Renderer& renderer) :
    refs(refs),
    defs(defs),
    win(bufferSize),
    renderer(renderer),

    hudBuilder(refs, defs, hudRoot),
    menuBuilder(refs, defs, menuRoot) {

    auto crosshair = std::make_shared<GuiRect>("crosshair");
    crosshair->create({22, 22}, {}, defs.textures["crosshair"]);
    crosshair->setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    hudRoot->add(crosshair);

    auto viginette = std::make_shared<GuiRect>("viginette");
    viginette->create(bufferSize, {}, defs.textures["viginette"]);
    hudRoot->add(viginette);

    handList->create({3, 3}, {}, {}, refs.getHand(), refs.getHand(), defs);
    menuRoot->add(handList);
}

void GameGui::update(double delta) {
    menuRoot->update(delta);
    hudRoot->update(delta);

    handList->setPos((renderer.window.getMousePos() - glm::ivec2(24)) / 3 * 3);
    menuRoot->handleMouseInput(renderer.window);
}

void GameGui::winResized(glm::ivec2 win) {
    this->win = win;

    hudRoot->get<GuiRect>("crosshair")->setPos({win.x / 2 - 11, win.y / 2 - 9});
    hudRoot->get<GuiRect>("viginette")->setScale(win);

    menuBuilder.build(win);
}

void GameGui::setMenu(const std::string& menu, const std::map<std::string, GuiBuilder::ComponentCallbacks>& callbacks) {
    menuState = "menu"; //TODO: Implement the menu state properly
    menuBuilder.setGui(menu, callbacks);
    menuBuilder.build(win);
}

void GameGui::closeMenu() {
    menuBuilder.clear();
    menuState = "";
}

const std::string &GameGui::getMenuState() {
    return menuState;
}

void GameGui::setVisible(bool visible) {
    menuRoot->setVisible(visible);
    hudRoot->setVisible(visible);
}

bool GameGui::isVisible() {
    return menuRoot->isVisible();
}

void GameGui::drawHud(Renderer &renderer) {
    hudRoot->draw(renderer);
}

void GameGui::drawMenu(Renderer &renderer) {
    menuRoot->draw(renderer);
}
