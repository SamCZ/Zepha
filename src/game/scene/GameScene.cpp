//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"

GameScene::GameScene(ClientState& state) : Scene(state),
    game(state.defs),
    refs(game.defs, net),
    world(game, &playerPos, &net),
    net(state.connection, game, player),
    player(world, game, state.renderer, refs),
    debugGui(state.renderer.window.getSize(), game) {

    state.renderer.setClearColor(148, 194, 240);
    state.renderer.window.lockMouse(true);

    game.init(world, player);
    world.init();
    net.init(&world);

    Packet r(PacketType::CONNECT_DATA_RECVD);
    r.sendTo(state.connection.getPeer(), PacketChannel::CONNECT);

    state.renderer.window.addResizeCallback("gamescene", [&](glm::ivec2 win) {
        debugGui.bufferResized(win);
    });
}

void GameScene::update() {
    game.update(state.deltaTime, state.renderer.window.keys);
    game.textures.update();
    net.update();

    Window& window = state.renderer.window;

    //Update Player
    player.update(window.input, state.deltaTime, window.getDelta());
    playerPos = player.getPos();

    for (auto entity : entities) entity->update(state.deltaTime);

    for (auto &chunkPacket : net.chunkPackets) world.loadChunkPacket(std::move(chunkPacket));
    net.chunkPackets.clear();

    debugGui.update(player, world, game, state.fps, world.getMeshChunkCount(), drawCalls, net.serverSideChunkGens, net.recvPackets);
    net.recvPackets = 0;
    world.update(state.deltaTime);

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
