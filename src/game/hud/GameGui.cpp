//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"
#include "components/compound/GUIInventoryList.h"

GameGui::GameGui(glm::vec2 bufferSize, LocalDefs& defs) :
    bufferSize(bufferSize),
    defs(defs),
    list(60, 12) {

    auto crosshair = std::make_shared<GUIRect>("crosshair");
    crosshair->create({22, 22}, {}, defs.textures["crosshair"]);
    crosshair->setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    builtIn.add(crosshair);

    auto viginette = std::make_shared<GUIRect>("viginette");
    viginette->create(bufferSize, {}, defs.textures["viginette"]);
    builtIn.add(viginette);

    menuRoot = std::make_shared<GUIContainer>("__lua_root");
    add(menuRoot);
}

void GameGui::bufferResized(glm::vec2 bufferSize) {
    this->bufferSize = bufferSize;

    builtIn.get<GUIRect>("crosshair")->setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    builtIn.get<GUIRect>("viginette")->setScale({bufferSize.x, bufferSize.y});

    menuRoot->empty();
//    recursivelyCreate(components, menuRoot);
}

void GameGui::setMenu(const std::string& menu) {
    menuState = "menu"; //TODO: Implement the menu state properly


}



void GameGui::closeMenu() {
    menuRoot->empty();
    menuState = "";
}

const std::string &GameGui::getMenuState() {
    return menuState;
}

void GameGui::setVisible(bool visible) {
    GUIComponent::setVisible(visible);
    builtIn.setVisible(visible);
}



void GameGui::drawViginette(Renderer &renderer) {
    builtIn.draw(renderer);
}
