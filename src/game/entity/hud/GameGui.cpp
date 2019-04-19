//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"

GameGui::GameGui(glm::vec2 bufferSize) {
    visible = true;

    crosshairTexture = new Texture((char*)"../res/tex/gui/crosshair.png");
    viginetteTexture = new Texture((char*)"../res/tex/gui/viginette.png");

    crosshair = new RectEntity(crosshairTexture);

    crosshair->setPos(glm::vec3(bufferSize.x / 2 - 11, bufferSize.y / 2 - 9, 0));
    crosshair->setScale(22);

    addDrawable(crosshair);

    viginette = new RectEntity(viginetteTexture);
    viginette->setScale(glm::vec3(bufferSize.x, bufferSize.y, 1));
    viginette->setPos(glm::vec3(0, 0, -5));

    addDrawable(viginette);
}

void GameGui::bufferResized(glm::vec2 bufferSize) {
    crosshair->setPos(glm::vec3(bufferSize.x / 2 - 11, bufferSize.y / 2 - 9, 0));
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

GameGui::~GameGui() {
    delete crosshairTexture;
    delete viginetteTexture;
}