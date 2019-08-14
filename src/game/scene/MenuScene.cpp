//
// Created by aurailus on 08/01/19.
//

#include "MenuScene.h"

MenuScene::MenuScene(ClientState& state) : Scene(state) {
    state.renderer.setClearColor(22, 22, 22);

    Font f(state.defs.textures(), state.defs.textures().getTextureRef("font"));

    auto alphaText = std::make_shared<GUIText>("alphaText");
    alphaText->create({2, 2}, {}, {1, 0, 0, 0}, {1, 1, 1, 1}, f);
    alphaText->setText("Alpha Software - Do Not Redistribute!");
    alphaText->setPos({8, 8});
    components.add(alphaText);

    auto creditText = std::make_shared<GUIText>("creditText");
    creditText->create({2, 2}, {}, {1, 0, 0, 0}, {0.6, 0.6, 0.6, 1}, f);
    creditText->setText("Created by Nicole Collings");
    creditText->setPos({8, 32});
    components.add(creditText);

    auto titleText = std::make_shared<GUIText>("titleText");
    titleText->create({12, 12}, {}, {}, {1, 1, 1, 1}, f);
    titleText->setText("Zepha");
    titleText->setPos({530, 120});
    components.add(titleText);

    auto mainMenuText = std::make_shared<GUIText>("mainMenuText");
    mainMenuText->create({4, 4}, {}, {0.7, 0.0, 0.0, 1}, {1, 1, 1, 1}, f);
    mainMenuText->setText("Main menu");
    mainMenuText->setPos({530, 230});
    components.add(mainMenuText);
}

void MenuScene::update() {
    if (state.renderer.resized) {
        auto size = state.renderer.getWindow().getSize();

        components.get<GUIText>("titleText")->setPos({size.x / 2 - 160, size.y / 2 - 120});
        components.get<GUIText>("mainMenuText")->setPos({size.x / 2 - 100, size.y / 2 + 20});

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