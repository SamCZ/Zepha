//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"

GameGui::GameGui(ClientState* state) {
    crosshairTexture = new Texture((char*)"../res/tex/gui/crosshair.png");
    crosshairTexture->load();

    crosshair = new RectEntity(crosshairTexture);

    float xx = state->renderer->getWindow()->getBufferWidth()/2;
    float yy = state->renderer->getWindow()->getBufferHeight()/2;

    crosshair->setPosition(glm::vec3(xx - 11, yy - 11, 0));
    crosshair->setScale(22);
}

void GameGui::pushGuiObjects(std::vector<Entity *> &list) {
    list.push_back(crosshair);
}

GameGui::~GameGui() {
    delete crosshair;
    delete crosshairTexture;
}
