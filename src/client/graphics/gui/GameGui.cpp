//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"

GameGui::GameGui(glm::vec2 bufferSize) {
    crosshairTexture = new Texture((char*)"../res/tex/gui/crosshair.png");
    crosshairTexture->load();

    crosshair = new RectEntity(crosshairTexture);

    crosshair->setPosition(glm::vec3(bufferSize.x/2 - 11, bufferSize.y/2 - 9, 0));
    crosshair->setScale(22);
}

void GameGui::pushGuiObjects(std::vector<Entity *> &list) {
    list.push_back(crosshair);
}

GameGui::~GameGui() {
    delete crosshair;
    delete crosshairTexture;
}

void GameGui::bufferResized(glm::vec2 bufferSize) {
    crosshair->setPosition(glm::vec3(bufferSize.x/2 - 11, bufferSize.y/2 - 9, 0));
}
