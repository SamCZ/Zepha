//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"
#include "../../def/entity/Model.h"

GameScene::GameScene(ClientState& state) : Scene(state),
    defs(state.defs),
    server(state.connection, defs),
    world(defs, &playerPos, &server),

    player(world, defs, state.renderer.getCamera()),

    gameGui (state.renderer.getCamera().getBufferDimensions(), defs.textures()),
    debugGui(state.renderer.getCamera().getBufferDimensions(), defs.textures()) {

    state.renderer.setClearColor(148, 194, 240);
    state.renderer.getWindow().lockMouse(true);

    defs.initLuaApi(world, gameGui);
    world.init();

    gui.push_back(&gameGui);
    gui.push_back(&debugGui);

    entities.push_back(&player);

    auto model = std::make_shared<Model>();
    model->import("/home/aurailus/Zepha/mods/default/models/rabbit.b3d", {defs.textures()["zeus:default:rabbit"]});

    rabbit = new Entity(model);
    rabbit->animState.defineAnimation("idle_sit", 1, 181);
    rabbit->animState.defineAnimation("hop", 182, 212);
    rabbit->animState.defineAnimation("sit_up", 213, 241);
    rabbit->animState.defineAnimation("idle_stand", 242, 401);
    rabbit->animState.setAnimation("idle_sit", 0, true);
    rabbit->animState.setPlaying(true);
    rabbit->setScale(1.f/16.f);
    rabbit->setPos({0, 40, 0});

    entities.push_back(rabbit);

    server.init(entities, &world);

    Packet r(PacketType::CONNECT_DATA_RECVD);
    r.sendTo(state.connection.getPeer(), PacketChannel::CONNECT);
}

void GameScene::update() {
    defs.update(static_cast<float>(state.deltaTime) * 1000);
    defs.textures().update();
    server.update(player);

    Window& window = state.renderer.getWindow();

    playerPos = player.getPos();
    player.update(window.input, state.deltaTime, window.getDeltaX(), window.getDeltaY());

    for (auto entity : entities) entity->update(state.deltaTime);
    rabbit->setAngle(rabbit->getAngle() + 1);

    if (state.renderer.resized) {
        debugGui.bufferResized(state.renderer.getCamera().getBufferDimensions());
        gameGui.bufferResized(state.renderer.getCamera().getBufferDimensions());

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
        gameGui.setVisible(hudVisible);
    }

    if (window.input.isKeyPressed(GLFW_KEY_F3)) {
        debugVisible = !debugVisible;
        debugGui.changeVisibilityState(hudVisible ? debugVisible ? 0 : 2 : 1);
    }

    if (window.input.isKeyPressed(GLFW_KEY_E)) {
        bool open = !gameGui.isInvOpen();
        gameGui.setInvOpen(open);
        window.lockMouse(!open);
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

    renderer.endDeferredCalls();
    renderer.beginGUIDrawCalls();
    renderer.enableTexture(&defs.textures().getAtlasTexture());

    for (auto entity : gui) entity->draw(renderer);

    renderer.swapBuffers();
}

void GameScene::cleanup() {
//    for (auto entity : entities) delete entity;
//    for (auto guiItem : gui) delete guiItem;
}