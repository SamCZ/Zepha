//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"

GameGui::GameGui(glm::vec2 bufferSize) :
    crosshairTexture(const_cast<char*>("../res/tex/gui/crosshair.png")),
    viginetteTexture(const_cast<char*>("../res/tex/gui/viginette.png")),
    crosshair(new TextureRect(&crosshairTexture)),
    viginette(new TextureRect(&viginetteTexture)) {

    crosshair->setPos(glm::vec3(bufferSize.x / 2 - 11, bufferSize.y / 2 - 9, 0));
    crosshair->setScale(22);

    addDrawable(crosshair);

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

    crosshair->setVisible(visible);
    viginette->setVisible(visible);
}