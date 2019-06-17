//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"

GameScene::GameScene(ClientState* state) : Scene(state),
    defs("../res/tex"),
    server("127.0.0.1", 12345, defs.textures()),
    world(defs, &playerPos, &server),

    player(world, defs, *state->renderer->getCamera()),

    gameGui(state->renderer->getCamera()->getBufferDimensions()),
    debugGui(state->renderer->getCamera()->getBufferDimensions(),  &defs.textures().getAtlasTexture()) {

    defs.init(world);
    world.init();

    gui.push_back(&gameGui);
    gui.push_back(&debugGui);

    entities.push_back(&player);

    server.init(entities, &world);
}


void GameScene::update() {
    defs.update(static_cast<float>(state->deltaTime) * 1000);
    defs.textures().update();
    server.update(player);

    auto window = state->renderer->getWindow();

    playerPos = player.getPos();
    player.update(window->input, state->deltaTime, window->getDeltaX(), window->getDeltaY());

    if (state->renderer->resized) {
        debugGui.bufferResized(state->renderer->getCamera()->getBufferDimensions());
        gameGui.bufferResized(state->renderer->getCamera()->getBufferDimensions());

        state->renderer->resized = false;
    }

    for (auto &chunkPacket : server.chunkPackets) world.loadChunkPacket(chunkPacket);
    server.chunkPackets.clear();

    debugGui.update(player, world, defs, state->fps, world.getMeshChunkCount(), drawCalls, server.serverSideChunkGens, server.recvPackets);
    world.update(state->deltaTime);

    if (window->input.isKeyPressed(GLFW_KEY_F1)) {
        hudVisible = !hudVisible;
        debugGui.changeVisibilityState(hudVisible ? debugVisible ? 0 : 2 : 1);
        gameGui.setVisible(hudVisible);
    }

    if (window->input.isKeyPressed(GLFW_KEY_F3)) {
        debugVisible = !debugVisible;
        debugGui.changeVisibilityState(hudVisible ? debugVisible ? 0 : 2 : 1);
    }
}

void GameScene::draw() {
    auto &renderer = *state->renderer;
    auto &camera = *renderer.getCamera();

    renderer.beginWorldDrawCalls();

    renderer.enableTexture(&defs.textures().getAtlasTexture());
    drawCalls = world.render(renderer);
    for (auto entity : entities) entity->draw(renderer);

    renderer.endWorldDrawCalls();
    renderer.beginGUIDrawCalls();

    for (auto entity : gui) entity->draw(renderer);

    renderer.swapBuffers();
}

void GameScene::cleanup() {
    //No pointers to clean up at this point
}