//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"

GameGui::GameGui(glm::vec2 bufferSize, TextureAtlas& atlas) {
    auto crosshair = std::make_shared<GUIRect>("__crosshair");
    crosshair->create({22, 22}, {}, atlas.getTextureRef("crosshair"));
    crosshair->setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    add(crosshair);

    auto viginette = std::make_shared<GUIRect>("__viginette");
    viginette->create(bufferSize, {}, atlas.getTextureRef("viginette"));
    add(viginette);

    auto root = std::make_shared<GUIContainer>("__root");
    add(root);
}

void GameGui::bufferResized(glm::vec2 bufferSize) {
    get<GUIRect>("__crosshair")->setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    get<GUIRect>("__viginette")->setScale({bufferSize.x, bufferSize.y});
}

void GameGui::setMenu(const std::string& state, std::shared_ptr<GUIComponent> component) {
    menuState = state;
    get<GUIContainer>("__root")->add(component);
}

void GameGui::closeMenu() {
    menuState = "";
    get<GUIContainer>("__root")->remove("__lua_root");
}

const std::string &GameGui::getMenuState() {
    return menuState;
}

void GameGui::setVisible(bool visible) {
    GUIComponent::setVisible(visible);
}