//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"

GameScene::GameScene(ClientState& state) : Scene(state),
    game(state.defs),
    world(game, &net),
    refs(game.defs, net),
    net(state.connection, game, player),
    player(world, game, state.renderer, refs),
    debugGui(state.renderer.window.getSize(), game) {

    namespace ph = std::placeholders;

    Packet r(PacketType::CONNECT_DATA_RECVD);
    r.sendTo(state.connection.getPeer(), PacketChannel::CONNECT);

    world.init(&player);
    net.init(&world, std::bind(&LocalInventoryRefs::packetReceived, refs, ph::_1));
    game.init(world, player);

    state.renderer.window.addResizeCallback("gamescene", std::bind(&DebugGui::bufferResized, debugGui, ph::_1));
    state.renderer.setClearColor(148, 194, 240);
    state.renderer.window.lockMouse(true);
}

void GameScene::update() {
    Window& window = state.renderer.window;

    player.update(window.input, state.delta, window.getDelta());
    game  .update(state.delta, state.renderer.window.keys);
    refs  .update(state.delta, net);
    net   .update();

    for (auto entity : entities) entity->update(state.delta);

    debugGui.update(player, world, game, state.fps, world.getMeshChunkCount(), drawCalls, net.serverSideChunkGens, net.recvPackets);
    world.update(state.delta);
    net.serverSideChunkGens = 0;
    net.recvPackets = 0;

    if (window.input.isKeyPressed(GLFW_KEY_F1)) {
        hudVisible = !hudVisible;
        debugGui.changeVisibilityState(hudVisible ? debugVisible ? 0 : 2 : 1);
        player.setGuiVisible(hudVisible);
    }

    if (window.input.isKeyPressed(GLFW_KEY_F3)) {
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
    player.draw(renderer);

    renderer.endDeferredCalls();
    renderer.beginGUIDrawCalls();
    renderer.enableTexture(&game.textures.atlasTexture);

    player.drawViginette(renderer);
    debugGui.draw(renderer);
    player.drawGUI(renderer);

    renderer.swapBuffers();
}

void GameScene::cleanup() {
    state.renderer.window.removeResizeCallback("gamescene");
}
