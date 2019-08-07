//
// Created by aurailus on 08/01/19.
//

#include "MenuScene.h"

MenuScene::MenuScene(ClientState& state) : Scene(state) {
    state.renderer.setClearColor(22, 22, 22);

    auto alphaText = std::make_shared<GUIText>("alphaText");
    alphaText->create({2, 2}, {}, {1, 0, 0, 0}, {}, state.defs.textures().getTextureRef("font"));
    alphaText->setText("Alpha Software - Do Not Redistribute!\nCreated by Nicole Collings");
    alphaText->setPos({8, 4});
    components.add(alphaText);

    auto titleText = std::make_shared<GUIText>("titleText");
    titleText->create({12, 12}, {}, {1, 0, 0, 1}, {}, state.defs.textures().getTextureRef("font"));
    titleText->setText("Zepha");
    titleText->setPos({490, 120});
    components.add(titleText);

    auto mainMenuText = std::make_shared<GUIText>("mainMenuText");
    mainMenuText->create({4, 4}, {}, {1, 0, 0, 1}, {}, state.defs.textures().getTextureRef("font"));
    mainMenuText->setText("Main menu");
    mainMenuText->setPos({530, 230});
    components.add(mainMenuText);
}

void MenuScene::update() {
    if (state.renderer.resized) {
        auto size = state.renderer.getWindow().getSize();

        components.get<GUIText>("titleText")->setPos({size.x / 2 - 220, size.y / 2 - 120});
        components.get<GUIText>("mainMenuText")->setPos({size.x / 2 - 130, size.y / 2});

        state.renderer.resized = false;
    }
}

void MenuScene::draw() {
    Renderer& renderer = state.renderer;

    renderer.beginChunkDeferredCalls();
    renderer.endDeferredCalls();
    renderer.beginGUIDrawCalls();
    renderer.enableTexture(&state.defs.textures().getAtlasTexture());

    components.draw(renderer);

    renderer.swapBuffers();
}