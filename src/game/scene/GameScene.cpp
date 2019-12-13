//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"

GameScene::GameScene(ClientState& state) : Scene(state),
    defs(state.defs),
    server(state.connection, defs),
    world(defs, &playerPos, &server),

    player(world, defs, state.renderer),
    debugGui(state.renderer.window.getSize(), defs) {

    state.renderer.setClearColor(148, 194, 240);
    state.renderer.window.lockMouse(true);

    defs.init(world, player);
    world.init();
    server.init(&world);

    Packet r(PacketType::CONNECT_DATA_RECVD);
    r.sendTo(state.connection.getPeer(), PacketChannel::CONNECT);

    state.renderer.window.addResizeCallback("gamescene", [&](glm::ivec2 win) {
        debugGui.bufferResized(win);
    });
}

void GameScene::update() {
    defs.update(state.deltaTime, state.renderer.window.keys);
    defs.textures.update();
    server.update(player);

    Window& window = state.renderer.window;

    //Update Player
    player.update(window.input, state.deltaTime, window.getDelta());
    playerPos = player.getPos();

    for (auto entity : entities) entity->update(state.deltaTime);

    for (auto &chunkPacket : server.chunkPackets) world.loadChunkPacket(std::move(chunkPacket));
    server.chunkPackets.clear();

    debugGui.update(player, world, defs, state.fps, world.getMeshChunkCount(), drawCalls, server.serverSideChunkGens, server.recvPackets);
    server.recvPackets = 0;
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
    renderer.enableTexture(&defs.textures.atlasTexture);

    drawCalls = world.renderChunks(renderer);

    renderer.beginEntityDeferredCalls();

    for (auto entity : entities) entity->draw(renderer);
    world.renderEntities(renderer);
    player.draw(renderer);

    renderer.endDeferredCalls();
    renderer.beginGUIDrawCalls();
    renderer.enableTexture(&defs.textures.atlasTexture);

    player.drawViginette(renderer);
    debugGui.draw(renderer);
    player.drawGUI(renderer);

    renderer.swapBuffers();
}

void GameScene::cleanup() {
    state.renderer.window.removeResizeCallback("gamescene");
}
