#include "LocalWorld.h"

#include "util/PerfTimer.h"
#include "util/net/PacketView.h"
#include "client/graph/Renderer.h"
#include "world/dim/chunk/Chunk.h"
#include "inv/LocalInventoryRefs.h"
#include "world/player/LocalPlayer.h"
#include "client/stream/WorldInterpolationStream.h"

LocalWorld::LocalWorld(SubgamePtr game, ServerConnection& conn, Renderer& renderer, vec<string>& perfSections) :
	World(game),
	renderer(renderer),
	net(conn, *this),
	refs(make_shared<LocalInventoryRefs>(game, net)),
//	debugGui(renderer.window.getSize(), game, *this, perfSections),
//	worldGenStream(make_shared<WorldInterpolationStream>(*game.l(), *this, 55)),
	player(make_shared<LocalPlayer>(game, *this, DimensionPtr(nullptr), renderer)) {
	
//	lock = renderer.window.onResize(Util::bind_this(&debugGui, &DebugGui::bufferResized));
}

void LocalWorld::init() {
	net.init(Util::bind_this(&(*refs), &LocalInventoryRefs::packetReceived));
	refs->init();
}

bool LocalWorld::updatePlayerDimension() {
	if (defaultDimension.empty()) return false;
	player->setDim(getDefaultDimension());
	activeDimension = getDefaultDimension().l();
	return true;
}

void LocalWorld::update(f64 delta, vec<usize>& perfTimings, PerfTimer& perf) {
	
	// Updates the dimensions.
	perf.start("update:world");
	for (auto& dimension : dimensions) dimension->update(delta);
	
	// Update children
	perf.start("update:player");
	if (*player) player.l()->update(renderer.window.input, delta, renderer.window.input.getMouseDelta());
	refs->update(delta, net);
	
	// Update the network
	perf.resume("other");
	net.update();
	
	// Update debug interface
	perf.start("update:debug");
//	debugGui.update(
//		player.l(), delta,
//		lastInterpolations, net.serverSideChunkGens, net.recvPackets,
//		perfTimings,
//		activeDimension->getMeshChunksDrawn(),
//		activeDimension->getMeshChunksCommitted());
	perf.resume("other");
	
	// Toggle regular interface
//	if (renderer.window.input.keyPressed(GLFW_KEY_F1)) {
//		hudVisible = !hudVisible;
//		player.l()->setHudVisible(hudVisible);
//		debugGui.changeVisibility(hudVisible ? debugVisible ? DebugGui::Visibility::ON :
//			DebugGui::Visibility::FPS_ONLY : DebugGui::Visibility::OFF);
//	}
//
//	// Toggle debug interface
//	if (renderer.window.input.keyPressed(GLFW_KEY_F3)) {
//		debugVisible = !debugVisible;
//		debugGui.changeVisibility(hudVisible ? debugVisible ? DebugGui::Visibility::ON :
//			DebugGui::Visibility::FPS_ONLY : DebugGui::Visibility::OFF);
//	}
}

void LocalWorld::handleWorldPacket(uptr<PacketView> p) {
	if (p->type == Packet::Type::CHUNK)
		commitChunk(make_shared<Chunk>(p->d.data));
	else if (p->type == Packet::Type::MAPBLOCK)
		while (!p->d.atEnd())
			commitChunk(make_shared<Chunk>(p->d.read<string>()));
}

void LocalWorld::handlePlayerEntPacket(uptr<PacketView> p) {
	if (!player) throw std::runtime_error("Received playerEnt info *before* the player was created.");
	
	u32 id = p->d.read<u32>();
	if (player->getId() == id) return;
	
	bool found = false;
	for (auto& entity : getActiveDimension().l()->playerEntities) {
		if (entity.getId() == id) {
			entity.interpPos(p->d.read<vec3>());
			entity.interpRotateZ(-p->d.read<f32>() + 90);
			entity.interpRotateY(-p->d.read<f32>() + 90);
			
			found = true;
			break;
		}
	}
	if (found) return;
	
	//TODO: Reimplement player models.
//    auto playerModel = std::make_shared<Model>();
//    playerModel->fromSerialized(static_cast<LocalSubgame&>(game).models.models["zeus:default:player"],
//        { static_cast<LocalSubgame&>(game).textures["zeus:default:player"] });
//    getActiveDimension().playerEntities.emplace_back(p->d.read<glm::vec3>(), id, playerModel);
}

void LocalWorld::handleModMessage(const string& channel, const string& message) {
	game->getParser().safe_function(game->getParser().core["trigger"], "message",
		channel, game->getParser().safe_function(game->getParser().core["deserialize"], message));
}

void LocalWorld::commitChunk(sptr<Chunk> c) {
	activeDimension->setChunk(std::move(c));
}

DimensionPtr LocalWorld::createDimension(const string& identifier, std::unordered_set<string>& biomes) {
	auto mapGen = std::make_shared<MapGen>(**game, *this, 0 /* TODO: Get the seed here */, biomes);
	dimensions.emplace_back(std::make_shared<LocalDimension>(
		game, *this, identifier, this->dimensions.size(), std::move(mapGen)));
	
	dimensionIndexes[identifier] = dimensions.size() - 1;
	DimensionPtr d = dimensions.back();
	return d;
}

void LocalWorld::sendMessage(const string& channel, const string& message) {
	net.sendPacket(Serializer().append(channel).append(message)
		.packet(Packet::Type::MOD_MESSAGE), Packet::Channel::INTERACT);
}

DimensionPtr LocalWorld::getActiveDimension() {
	return activeDimension;
}

void LocalWorld::setActiveDimension(DimensionPtr dim) {
	activeDimension->deactivate();
	activeDimension = dim.l();
}

ClientNetworkInterpreter& LocalWorld::getNet() {
	return net;
}

PlayerPtr LocalWorld::getPlayer() {
	return player;
}

InventoryRefsPtr LocalWorld::getRefs() {
	return refs;
}

void LocalWorld::drawChunks() {
	activeDimension->renderChunks(renderer);
}

void LocalWorld::drawEntities() {
	activeDimension->renderEntities(renderer);
	player.l()->draw(renderer);
}

void LocalWorld::drawInterface() {
	player.l()->drawHud(renderer);
//	debugGui.draw(renderer);
	player.l()->drawMenu(renderer);
}
