//
// Created by aurailus on 14/12/18.
//

#include "LocalWorld.h"

#include "util/net/PacketView.h"
#include "client/graph/Renderer.h"
#include "world/player/LocalPlayer.h"
#include "client/stream/WorldInterpolationStream.h"

LocalWorld::LocalWorld(SubgamePtr game, ServerConnection& conn, Renderer& renderer, vec<string>& perfSections) :
	World(game),
	renderer(renderer),
	net(conn, *this),
	debugGui(renderer.window.getSize(), game, *this, perfSections),
	refs(std::make_shared<LocalInventoryRefs>(game, net)),
	worldGenStream(std::make_shared<WorldInterpolationStream>(*game.l(), *this, 55)),
	player(std::make_shared<LocalPlayer>(game, *this, DimensionPtr(nullptr), renderer)) {}

void LocalWorld::connect() {
	net.init(Util::bind_this(&(*refs), &LocalInventoryRefs::packetReceived));
	refs->init();
}

bool LocalWorld::updatePlayerDimension() {
	if (defaultDimension.empty()) return false;
	player->setDim(getDefaultDimension());
	activeDimension = getDefaultDimension().l();
	return true;
}

void LocalWorld::update(double delta, vec<usize>& perfTimings, PerfTimer& perf) {
	perf.start("update:world");
	World::update(delta);
	
	// Update children
	
	perf.start("update:player");
	if (*player) player.l()->update(renderer.window.input, delta, renderer.window.input.mouseDelta());
	refs->update(delta, net);
	perf.start("update:net");
	net.update();
	
	// Commit interpolated mapblocks
	
	perf.start("update:chunks");
	auto finishedChunks = worldGenStream->update();
	lastInterpolations = finishedChunks->size() / 64;
	for (const auto& chunk : *finishedChunks) commitChunk(chunk);
	
	// Update debug interface
	
	perf.start("update:debug");
	debugGui.update(
		player.l(), delta,
		lastInterpolations, net.serverSideChunkGens, net.recvPackets,
		perfTimings,
		activeDimension->getMeshChunksDrawn(),
		activeDimension->getMeshChunksCommitted());
	
	// Toggle regular interface
	
	if (renderer.window.input.keyPressed(GLFW_KEY_F1)) {
		hudVisible = !hudVisible;
		debugGui.changeVisibility(hudVisible ? debugVisible ? DebugGui::Visibility::OFF :
			DebugGui::Visibility::FPS_ONLY : DebugGui::Visibility::ON);
		player.l()->setHudVisible(hudVisible);
	}
	
	// Toggle debug interface
	
	if (renderer.window.input.keyPressed(GLFW_KEY_F3)) {
		debugVisible = !debugVisible;
		debugGui.changeVisibility(hudVisible ? debugVisible ? DebugGui::Visibility::OFF :
			DebugGui::Visibility::FPS_ONLY : DebugGui::Visibility::ON);
	}
}

void LocalWorld::handleWorldPacket(std::unique_ptr<PacketView> p) {
	worldGenStream->queuePacket(std::move(p));
}

void LocalWorld::handlePlayerEntPacket(std::unique_ptr<PacketView> p) {
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

void LocalWorld::handleModMessage(const std::string& channel, const std::string& message) {
	game->getParser().safe_function(game->getParser().core["trigger"], "message",
		channel, game->getParser().safe_function(game->getParser().core["deserialize"], message));
}

void LocalWorld::commitChunk(std::shared_ptr<Chunk> c) {
	activeDimension->setChunk(std::move(c));
}

DimensionPtr LocalWorld::createDimension(const std::string& identifier, std::unordered_set<std::string>& biomes) {
	auto mapGen = std::make_shared<MapGen>(**game, *this, 0 /* TODO: Get the seed here */, biomes);
	dimensions.emplace_back(std::make_shared<LocalDimension>(
		game, *this, identifier, this->dimensions.size(), std::move(mapGen)));
	
	DimensionPtr d = dimensions.back();
	return d;
}

void LocalWorld::sendMessage(const std::string& channel, const std::string& message) {
	net.sendPacket(Serializer().append(channel).append(message)
		.packet(Packet::Type::MOD_MESSAGE), Packet::Channel::INTERACT);
}

DimensionPtr LocalWorld::getActiveDimension() {
	return activeDimension;
}

void LocalWorld::setActiveDimension(DimensionPtr dim) {
	this->activeDimension->deactivate();
	this->activeDimension = dim.l();
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

void LocalWorld::drawWorld() {
	activeDimension->renderChunks(renderer);
}

void LocalWorld::drawEntities() {
	activeDimension->renderEntities(renderer);
	player.l()->draw(renderer);
}

void LocalWorld::drawInterface() {
	player.l()->drawHud(renderer);
	debugGui.draw(renderer);
	player.l()->drawMenu(renderer);
}
