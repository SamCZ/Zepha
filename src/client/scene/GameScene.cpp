#include "GameScene.h"

#include "util/Util.h"
#include "client/Client.h"
#include "util/PerfTimer.h"
#include "util/net/PacketView.h"
#include "client/graph/Renderer.h"

GameScene::GameScene(Client& client) : Scene(client),
	world(make_shared<LocalWorld>(client.game, client.connection, client.renderer, perfSections)) {
	
	Packet r(Packet::Type::CONNECT_DATA_RECVD);
	r.sendTo(client.connection.getPeer(), Packet::Channel::CONNECT);
	
	world.l()->init();
	client.game->init(world, world.l()->getPlayer(), client);
	world.l()->updatePlayerDimension();
	
	client.renderer.setClearColor(148, 194, 240);
//	client.renderer.setClearColor(16, 24, 32);
	client.renderer.window.input.lockMouse(true);
}

void GameScene::update() {
	Window& window = client.renderer.window;
	auto perfTimings = perf.end();
	
	perf.start("update:mods");
	client.game->update(client.getDelta());
	
	world.l()->update(client.getDelta(), perfTimings, perf);
	
	world.l()->getNet().serverSideChunkGens = 0;
	world.l()->getNet().recvPackets = 0;
}

void GameScene::draw() {
	Renderer& renderer = client.renderer;
	Camera& camera = renderer.camera;
	
	perf.start("draw:world");
	renderer.beginChunkDeferredCalls();
	renderer.enableTexture(&client.game->textures.atlasTexture);
	world.l()->drawChunks();
	
	perf.start("draw:entities");
	renderer.beginEntityDeferredCalls();
	renderer.enableTexture(&client.game->textures.atlasTexture);
	world.l()->drawEntities();
	renderer.endDeferredCalls();
	
	perf.start("draw:interface");
	renderer.beginGUIDrawCalls();
	renderer.enableTexture(&client.game->textures.atlasTexture);
	world.l()->drawInterface();
	
	perf.start("idle");
}
