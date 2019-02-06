//
// Created by aurailus on 05/02/19.
//

#include "GameGui.h"

GameGui::GameGui(ClientState* state) {
    crosshairTexture = new Texture((char*)"../res/tex/gui/crosshair.png");
    crosshairTexture->load();

    crosshair = new Entity();

    auto crossVerts = new std::vector<float> {
            -0.5, -0.5, 0, 0, 0, 0, 0, 0,
            -0.5,  0.5, 0, 0, 1, 0, 0, 0,
             0.5,  0.5, 0, 1, 1, 0, 0, 0,
             0.5, -0.5, 0, 1, 0, 0, 0, 0,
    };
    auto crossInds = new std::vector<unsigned int> {
            0, 1, 2, 2, 3, 0
    };

    auto m = new Mesh();
    m->create(crossVerts, crossInds);

    crosshair->create(m, crosshairTexture);

    delete crossVerts;
    delete crossInds;

    float xx = state->renderer->getWindow()->getBufferWidth()/2;
    float yy = state->renderer->getWindow()->getBufferHeight()/2;

    crosshair->setPosition(glm::vec3(xx, yy, 0));
    crosshair->setScale(22);
}

void GameGui::pushGuiObjects(std::vector<Entity *> &list) {
    list.push_back(crosshair);
}

GameGui::~GameGui() {
    delete crosshair;
    delete crosshairTexture;
}
