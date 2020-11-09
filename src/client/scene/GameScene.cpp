//
// Created by aurailus on 17/12/18.
//

#include "GameScene.h"

#include "client/Client.h"
#include "util/net/PacketView.h"
#include "client/graph/Renderer.h"

GameScene::GameScene(Client& client) : Scene(client),
	world(std::make_shared<LocalWorld>(client.game, client.connection, client.renderer)),
	debugGui(client.renderer.window.getSize(), client.game, world) {
	
	Packet r(Packet::Type::CONNECT_DATA_RECVD);
	r.sendTo(client.connection.getPeer(), Packet::Channel::CONNECT);
	
	world.l()->connect();
	client.game->init(world, world.l()->getPlayer(), client);
	world.l()->updatePlayerDimension();
	
	client.renderer.window.addResizeCallback("gamescene", Util::bind_this(&debugGui, &DebugGui::bufferResized));
	client.renderer.setClearColor(148, 194, 240);
	client.renderer.window.input.lockMouse(true);
}

void GameScene::update() {
	Window& window = client.renderer.window;
	
	client.game->update(client.getDelta());
	world->update(client.getDelta());
	
	for (auto entity : entities) entity->update(client.getDelta());
	
	double lastFps = 1 / client.getDelta();
	debugGui.update(world.l()->getPlayer().l(), lastFps, world.l()->getActiveDimension().l()->getMeshChunkCount(),
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
	Renderer& renderer = client.renderer;
	Camera& camera = renderer.camera;
	
	renderer.beginChunkDeferredCalls();
	renderer.enableTexture(&client.game->textures.atlasTexture);
	
	drawCalls = world.l()->renderChunks(renderer);
	
	renderer.beginEntityDeferredCalls();
	
	for (auto entity : entities) entity->draw(renderer);
	world.l()->renderEntities(renderer);
	
	renderer.endDeferredCalls();
	renderer.beginGUIDrawCalls();
	renderer.enableTexture(&client.game->textures.atlasTexture);
	
	world.l()->getPlayer().l()->drawHud(renderer);
	debugGui.draw(renderer);
	world.l()->getPlayer().l()->drawMenu(renderer);
	
	renderer.swapBuffers();
}

void GameScene::cleanup() {
	client.renderer.window.removeResizeCallback("gamescene");
}
