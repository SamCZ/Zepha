//
// Created by aurailus on 2019-12-28.
//

#include "LuaErrorScene.h"

#include "../graph/Renderer.h"
#include "../../def/texture/Font.h"
#include "../hud/components/basic/GuiRect.h"
#include "../hud/components/basic/GuiText.h"

LuaErrorScene::LuaErrorScene(ClientState &state, const std::string &err) : Scene(state), err(err) {
    state.renderer.setClearColor(0, 0, 0);
    state.renderer.window.input.lockMouse(false);

    Font f(state.defs.textures, state.defs.textures["font"]);
    glm::ivec2 win = state.renderer.window.getSize();

    auto container = std::make_shared<GuiRect>("container");
    container->create({800, 500}, {}, {0.05, 0.05, 0.05, 1});
    container->setPos({win.x / 2 - 800 / 2, win.y / 2 - 500 / 2});
    components.add(container);

    auto titleText = std::make_shared<GuiText>("titleText");
    titleText->create({3, 3}, {}, {}, {1, 0.4, 0.5, 1}, f);
    titleText->setText("The Zepha sandbox has encountered an error.");
    titleText->setPos({16, 12});
    container->add(titleText);

    auto errorText = std::make_shared<GuiText>("errorText");
    errorText->create({2, 2}, {}, {}, {0.85, 0.85, 0.85, 1}, f);
    errorText->setText(err);
    errorText->setPos({16, 48});
    container->add(errorText);

    state.renderer.window.addResizeCallback("scene", [&](glm::ivec2 win) {
        components.get<GuiRect>("container")->setPos({win.x / 2 - 800 / 2, win.y / 2 - 500 / 2});
    });
}

void LuaErrorScene::update() {
    state.defs.textures.update();
}

void LuaErrorScene::draw() {
    Renderer& renderer = state.renderer;

    renderer.beginChunkDeferredCalls();
    renderer.endDeferredCalls();
    renderer.beginGUIDrawCalls();
    renderer.enableTexture(&state.defs.textures.atlasTexture);

    components.draw(renderer);

    renderer.swapBuffers();
}

void LuaErrorScene::cleanup() {
    state.renderer.window.removeResizeCallback("scene");
}