//
// Created by aurailus on 08/01/19.
//

#include "MenuScene.h"

MenuScene::MenuScene(ClientState *state) : Scene(state) {
    fontTexture = new Texture((char*)"../tex/gui/font.png");
    fontTexture->load();

    auto alphaText = new HudText(fontTexture);
    alphaText->set("Zeus Alpha 0.01");
    alphaText->setScale(3);
    alphaText->setPosition(glm::vec3(8, 4, 0));
    entities.push_back(alphaText);

    auto titleText = new HudText(fontTexture);
    titleText->set("Zeus");
    titleText->setScale(12);
    titleText->setPosition(glm::vec3(490, 120, 0));
    entities.push_back(titleText);

    auto mainMenuText = new HudText(fontTexture);
    mainMenuText->set("MAIN MENU");
    mainMenuText->setScale(4);
    mainMenuText->setPosition(glm::vec3(530, 230, 0));
    entities.push_back(mainMenuText);
}

void MenuScene::update() {
    //Nothing
}

void MenuScene::draw() {
    state->renderer->begin();

    state->renderer->enableGuiShader();

    Texture* prevTexture = nullptr;

    for (auto &element : entities) {
        if (element->getTexture() != prevTexture) {
            prevTexture = element->getTexture();
            prevTexture->use();
        }

        state->renderer->drawGui(element);
    }

    state->renderer->end();
}

void MenuScene::cleanup() {
    fontTexture->clear();
    delete fontTexture;
}