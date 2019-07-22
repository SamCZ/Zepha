//
// Created by aurailus on 08/01/19.
//

#include "MenuScene.h"

MenuScene::MenuScene(ClientState& state) : Scene(state),
    fontTexture((char*)"../res/tex/gui/font.png"),
    alphaText(&fontTexture),
    titleText(&fontTexture),
    mainMenuText(&fontTexture) {

    state.renderer.setClearColor(22, 22, 22);

    alphaText.set("Alpha Software - Do Not Redistribute!\nCreated by Nicole Collings");
    alphaText.setScale(2);
    alphaText.setPos(glm::vec3(8, 4, 0));

    titleText.set("Zeus");
    titleText.setScale(12);
    titleText.setPos(glm::vec3(490, 120, 0));

    mainMenuText.set("MAIN MENU");
    mainMenuText.setScale(4);
    mainMenuText.setPos(glm::vec3(530, 230, 0));
}

void MenuScene::update() {
    if (state.renderer.resized) {
        auto size = state.renderer.getWindow().getSize();

        titleText.setPos({size.x / 2 - 180, size.y / 2 - 120, 0});
        mainMenuText.setPos({size.x / 2 - 130, size.y / 2, 0});

        state.renderer.resized = false;
    }
}

void MenuScene::draw() {
    Renderer& renderer = state.renderer;

    renderer.beginChunkDeferredCalls();
    renderer.endDeferredCalls();
    renderer.beginGUIDrawCalls();

    alphaText.draw(renderer);
    titleText.draw(renderer);
    mainMenuText.draw(renderer);

    renderer.swapBuffers();
}