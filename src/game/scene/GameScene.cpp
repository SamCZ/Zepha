//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"

#include "../ClientState.h"
#include "../../net/Packet.h"
#include "../graph/Renderer.h"
#include "../../net/PacketView.h"

GameScene::GameScene(ClientState& state) : Scene(state),
    game(state.defs),
    world(game, state.connection, state.renderer),
    debugGui(state.renderer.window.getSize(), game, world) {

    Packet r(PacketType::CONNECT_DATA_RECVD);
    r.sendTo(state.connection.getPeer(), PacketChannel::CONNECT);

    world.connect();
    game.initApi(world, state);
    if (world.initPlayer()) game.loadPlayer(world.getPlayer());

    state.renderer.window.addResizeCallback("gamescene", Util::bind_this(&debugGui, &DebugGui::bufferResized));
    state.renderer.setClearColor(148, 194, 240);
    state.renderer.window.input.lockMouse(true);
}

void GameScene::update() {
    Window& window = state.renderer.window;

    game.update(state.delta);
    world.update(state.delta);

    for (auto entity : entities) entity->update(state.delta);

    debugGui.update(*world.getPlayer(), state.fps, world.getActiveDimension().getMeshChunkCount(),
        drawCalls, world.getNet().serverSideChunkGens, world.getNet().recvPackets);

    world.getNet().serverSideChunkGens = 0;
    world.getNet().recvPackets = 0;

    if (window.input.keyPressed(GLFW_KEY_F1)) {
        hudVisible = !hudVisible;
        debugGui.changeVisibilityState(hudVisible ? debugVisible ? 0 : 2 : 1);
        world.getPlayer()->setHudVisible(hudVisible);
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
    renderer.enableTexture(&game.textures.atlasTexture);

    drawCalls = world.renderChunks(renderer);

    renderer.beginEntityDeferredCalls();

    for (auto entity : entities) entity->draw(renderer);
    world.renderEntities(renderer);

    renderer.endDeferredCalls();
    renderer.beginGUIDrawCalls();
    renderer.enableTexture(&game.textures.atlasTexture);

    world.getPlayer()->drawHud(renderer);
    debugGui.draw(renderer);
    world.getPlayer()->drawMenu(renderer);

    renderer.swapBuffers();
}

void GameScene::cleanup() {
    state.renderer.window.removeResizeCallback("gamescene");
}
