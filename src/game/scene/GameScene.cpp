//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"
#include "../entity/Model.h"

GameScene::GameScene(ClientState& state) : Scene(state),
    defs(state.defs),
    server(state.connection, defs),
    world(defs, &playerPos, &server),

    player(world, defs, state.renderer),
    debugGui(state.renderer.getCamera().getBufferDimensions(), defs) {

    state.renderer.setClearColor(148, 194, 240);
    state.renderer.getWindow().lockMouse(true);

    defs.initLuaApi(world, player);
    world.init();
    server.init(&world);

    Packet r(PacketType::CONNECT_DATA_RECVD);
    r.sendTo(state.connection.getPeer(), PacketChannel::CONNECT);
}

void GameScene::update() {
    defs.update(state.deltaTime, state.renderer.getWindow().keys);
    defs.textures().update();
    server.update(player);

    Window& window = state.renderer.getWindow();

    //Update Player
    player.update(window.input, state.deltaTime, window.getDeltaX(), window.getDeltaY());
    playerPos = player.getPos();

    for (auto entity : entities) entity->update(state.deltaTime);

    if (state.renderer.resized) {
        debugGui.bufferResized(state.renderer.getCamera().getBufferDimensions());
        state.renderer.resized = false;
    }

    for (auto &chunkPacket : server.chunkPackets) world.loadChunkPacket(chunkPacket);
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
    Camera& camera = renderer.getCamera();

    renderer.beginChunkDeferredCalls();
    renderer.enableTexture(&defs.textures().getAtlasTexture());

    drawCalls = world.renderChunks(renderer);

    renderer.beginEntityDeferredCalls();

    for (auto entity : entities) entity->draw(renderer);
    world.renderEntities(renderer);
    player.draw(renderer);

    renderer.endDeferredCalls();
    renderer.beginGUIDrawCalls();
    renderer.enableTexture(&defs.textures().getAtlasTexture());

    player.drawGUI(renderer);
    debugGui.draw(renderer);

    renderer.swapBuffers();
}