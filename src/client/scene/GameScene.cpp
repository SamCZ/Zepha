//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"

#include "client/ClientState.h"
#include "util/net/PacketView.h"
#include "client/graph/Renderer.h"

GameScene::GameScene(ClientState& state) : Scene(state),
    game(std::make_shared<LocalSubgame>(state.game)),
    world(std::make_shared<LocalWorld>(game, state.connection, state.renderer)),
    debugGui(state.renderer.window.getSize(), game, world) {

    Packet r(Packet::Type::CONNECT_DATA_RECVD);
    r.sendTo(state.connection.getPeer(), Packet::Channel::CONNECT);

    world.l()->connect();
    game .l()->init(world, world.l()->getPlayer(), state);
    world.l()->updatePlayerDimension();

    state.renderer.window.addResizeCallback("gamescene", Util::bind_this(&debugGui, &DebugGui::bufferResized));
    state.renderer.setClearColor(148, 194, 240);
    state.renderer.window.input.lockMouse(true);
}

void GameScene::update() {
    Window& window = state.renderer.window;

    game.l()->update(state.delta);
    world->update(state.delta);

    for (auto entity : entities) entity->update(state.delta);

    debugGui.update(world.l()->getPlayer().l(), state.fps, world.l()->getActiveDimension().l()->getMeshChunkCount(),
        drawCalls, world.l()->getNet().serverSideChunkGens, world.l()->getNet().recvPackets);

    world.l()->getNet().serverSideChunkGens = 0;
    world.l()->getNet().recvPackets = 0;

    if (window.input.keyPressed(GLFW_KEY_F1)) {
        hudVisible = !hudVisible;
        debugGui.changeVisibilityState(hudVisible ? debugVisible ? 0 : 2 : 1);
        world.l()->getPlayer().l()->setHudVisible(hudVisible);
    }

    if (window.input.keyPressed(GLFW_KEY_F3)) {
        debugVisible = !debugVisible;
        debugGui.changeVisibilityState(hudVisible ? debugVisible ? 0 : 2 : 1);
    }
}

void GameScene::draw() {
    Renderer& renderer = state.renderer;
    Camera& camera = renderer.camera;

    renderer.beginChunkDeferredCalls();
    renderer.enableTexture(&game.l()->textures.atlasTexture);

    drawCalls = world.l()->renderChunks(renderer);

    renderer.beginEntityDeferredCalls();

    for (auto entity : entities) entity->draw(renderer);
    world.l()->renderEntities(renderer);

    renderer.endDeferredCalls();
    renderer.beginGUIDrawCalls();
    renderer.enableTexture(&game.l()->textures.atlasTexture);

    world.l()->getPlayer().l()->drawHud(renderer);
    debugGui.draw(renderer);
    world.l()->getPlayer().l()->drawMenu(renderer);

    renderer.swapBuffers();
}

void GameScene::cleanup() {
    state.renderer.window.removeResizeCallback("gamescene");
}
