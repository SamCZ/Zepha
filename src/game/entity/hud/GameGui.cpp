//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"

GameGui::GameGui(glm::vec2 bufferSize, TextureAtlas& atlas) :
    crosshairRef(atlas.getTextureRef("crosshair")),
    viginetteRef(atlas.getTextureRef("viginette")) {

    crosshair.create({22, 22}, {}, crosshairRef);
    crosshair.setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    viginette.create(bufferSize, {}, viginetteRef);
}

void GameGui::draw(Renderer &renderer) {
    viginette.draw(renderer);
    crosshair.draw(renderer);
}

void GameGui::bufferResized(glm::vec2 bufferSize) {
    crosshair.setPos({bufferSize.x / 2 - 11, bufferSize.y / 2 - 9});
    viginette.setScale({bufferSize.x, bufferSize.y});
}

void GameGui::setVisible(bool visible) {
    this->visible = visible;

    crosshair.setVisible(visible);
    viginette.setVisible(visible);
}
