//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"

GameGui::GameGui(glm::vec2 bufferSize) {
    visible = true;

    crosshairTexture = new Texture((char*)"../res/tex/gui/crosshair.png");
    crosshairTexture->load();

    viginetteTexture = new Texture((char*)"../res/tex/gui/viginette.png");
    viginetteTexture->load();

    crosshair = new RectEntity(crosshairTexture);

    crosshair->setPosition(glm::vec3(bufferSize.x/2 - 11, bufferSize.y/2 - 9, 0));
    crosshair->setScale(22);

    viginette = new RectEntity(viginetteTexture);
    viginette->setScale(glm::vec3(bufferSize.x, bufferSize.y, 1));
    viginette->setPosition(glm::vec3(0, 0, -5));
}

void GameGui::pushGuiObjects(std::vector<Entity *> &list) {
    list.push_back(viginette);
    list.push_back(crosshair);
}

GameGui::~GameGui() {
    delete crosshair;
    delete crosshairTexture;
    delete viginette;
    delete viginetteTexture;
}

void GameGui::bufferResized(glm::vec2 bufferSize) {
    crosshair->setPosition(glm::vec3(bufferSize.x/2 - 11, bufferSize.y/2 - 9, 0));
    viginette->setScale(glm::vec3(bufferSize.x, bufferSize.y, 1));
}

void GameGui::setVisible(bool visible) {
    this->visible = visible;

    this->crosshair->setVisible(visible);
    this->viginette->setVisible(visible);
}

bool GameGui::isVisible() {
    return visible;
}