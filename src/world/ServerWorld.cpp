//
// Created by aurailus on 05/03/19.
//

#include <iostream>
#include <glm/glm.hpp>
#include <unordered_map>

#include "ServerWorld.h"

#include "util/Bounds.h"
#include "game/def/BlockDef.h"
#include "lua/usertype/Target.h"
#include "util/net/Serializer.h"
#include "server/ServerClient.h"
#include "server/ServerClients.h"
#include "world/dim/chunk/Chunk.h"
#include "world/dim/chunk/MapBlock.h"
#include "world/player/ServerPlayer.h"
#include "world/inv/ServerInventoryRefs.h"
#include "world/dim/ent/ServerLuaEntity.h"
#include "server/stream/ServerGenStream.h"
#include "server/stream/ServerPacketStream.h"

ServerWorld::ServerWorld(unsigned int seed, SubgamePtr game, ServerClients& clients) :
	World(game),
	seed(seed),
	clients(clients),
	refs(std::make_shared<ServerInventoryRefs>(game, clients)) {
	clients.init(this);
	
	generateOrder.reserve(mapBlockGenRange.x * 2 + 1 * mapBlockGenRange.x * 2 + 1 * mapBlockGenRange.y * 2 + 1);
	std::unordered_set<glm::ivec3, Vec::ivec3> found {};
	std::queue<glm::ivec3> queue {};
	
	queue.emplace(0, 0, 0);
	found.emplace(0, 0, 0);
	
	const std::vector<glm::ivec3> dirs{
		ivec3 { 1, 0, 0 }, ivec3{ -1, 0, 0 },
		ivec3 { 0, 1, 0 }, ivec3{ 0, -1, 0 },
		ivec3 { 0, 0, 1 }, ivec3{ 0, 0, -1 }};
	
	while (!queue.empty()) {
		glm::ivec3 pos = queue.front();
		queue.pop();
		
		generateOrder.push_back(pos);
		
		for (auto dir : dirs) {
			glm::ivec3 offset = pos + dir;
			if (offset.x < -mapBlockGenRange.x || offset.x > mapBlockGenRange.x ||
			    offset.y < -mapBlockGenRange.y || offset.y > mapBlockGenRange.y ||
			    offset.z < -mapBlockGenRange.x || offset.z > mapBlockGenRange.x ||
			    found.count(offset))
				continue;
			else {
				found.insert(offset);
				queue.push(offset);
			}
		}
	}
}

void ServerWorld::init(const std::string& worldDir) {
	genStream = std::make_unique<ServerGenStream>(*game.s(), *this);
	packetStream = std::make_unique<ServerPacketStream>(*this);
//    fileManip = std::make_shared<FileManipulator>("worlds/" + worldDir + "/");
}

void ServerWorld::update(double delta) {
	World::update(delta);
	refs->update();
	
	u32 genCount = 0;
	std::unordered_set<ivec4, Vec::ivec4> updatedChunks {};
	
	auto finishedGen = genStream->update();
//	if (finishedGen->size()) std::cout << finishedGen->size() << " finished gens" << std::endl;
	for (auto& data : *finishedGen) {
		for (const auto& chunkPair : *data.created) {
			updatedChunks.insert(ivec4(chunkPair.first, data.dim));
			getDimension(data.dim)->setChunk(sptr<Chunk>(chunkPair.second));
		}
		
		// Mapblock might have been pruned in between generation assignment and now.
		auto mb = getDimension(data.dim)->getMapBlock(glm::ivec3(data.pos));
		if (mb) mb->generated = true;
		
		packetStream->queue(data.dim, data.pos);
		genCount++;
	}
	
	auto finishedPackets = packetStream->update();
//	if (finishedPackets->size()) std::cout << finishedPackets->size() << " finished packets" << std::endl;
	for (auto& data : *finishedPackets) {
		for (auto& client : clients.getClients()) {
			if (!client.second->player) continue;
			data->packet->sendTo(client.second->peer, Packet::Channel::WORLD);
		}
	}
	
	generatedMapBlocks = genCount;

//    for (auto& chunkPos : updatedChunks) {
//    	std::cout << Util::toString(chunkPos) << std::endl;
//        ivec3 mapBlockPos = Space::MapBlock::world::fromChunk(chunkPos);
//        auto dim = getDimension(chunkPos.w);
//        auto chunk = dim->getChunk(ivec3(chunkPos));
//
//        assert(chunk != nullptr);
//
////        bool sentAlready = false;
////        for (auto& mapBlock : generatedMapBlocks) if (mapBlock == mapBlockPos) { sentAlready = true; break; }
////        if (sentAlready) continue;
//
//        Packet p(Packet::Type::CHUNK);
//        p.data = chunk->compress();
//
//        for (auto& client : clients.getClients()) {
//            if (!client.second->player) continue;
//
//            auto myChunk = Space::Chunk::world::fromBlock(client.second->player->getPos());
//
//            std::pair<ivec3, ivec3> bounds = {
//                {myChunk.x - activeChunkRange.x, myChunk.y - activeChunkRange.y, myChunk.z - activeChunkRange.x},
//                {myChunk.x + activeChunkRange.x, myChunk.y + activeChunkRange.y, myChunk.z + activeChunkRange.x}};
//
////            if (isInBounds(chunkPos, bounds))
//            p.sendTo(client.second->peer, Packet::Channel::WORLD);
//        }
//    }
	
	Packet r = Serializer().append(generatedMapBlocks).packet(Packet::Type::SERVER_INFO);
	
	for (auto& client : clients.getClients()) {
		if (!client.second->player) return;
		r.sendTo(client.second->player->getPeer(), Packet::Channel::SERVER);
		if (client.second->player->changedMapBlocks) changedMapBlocks(*client.second->player);
	}
	
	for (auto& d : dimensions) {
		auto dimension = std::static_pointer_cast<ServerDimension>(d);
		u16 ind = dimension->getInd();
		
		// Update clients with new entity information.
		
		Serializer inf;
		inf.append(ind);
		
		for (auto& entity : dimension->getLuaEntities()) {
			auto str = entity.entity.s()->serialize();
			if (!str.empty()) inf.append(str);
		}
		
		// Contains more than just the dimension identifier.
		if (inf.data.size() > sizeof(u16)) {
			auto p = inf.packet(Packet::Type::ENTITY_INFO);
			for (auto& client : clients.getClients())
				if (client.second->player && client.second->player->getDim()->getInd() == ind)
					p.sendTo(client.second->peer, Packet::Channel::ENTITY);
		}
		
		// Update clients with removed entities.
		
		Serializer rem;
		rem.append(ind);
		for (i64 entity : dimension->getRemovedEntities()) rem.append<i64>(entity);
		
		if (rem.data.size() > sizeof(u16)) {
			Packet p = rem.packet(Packet::Type::ENTITY_REMOVED);
			for (auto& client : clients.getClients())
				if (client.second->player && client.second->player->getDim()->getInd() == ind)
					p.sendTo(client.second->player->getPeer(), Packet::Channel::ENTITY);
		}
		
		dimension->clearRemovedEntities();
	}
}

DimensionPtr ServerWorld::createDimension(const std::string& identifier, std::unordered_set<std::string>& biomes) {
	auto mapGen = std::make_shared<MapGen>(**game, *this, seed, biomes);
	dimensions.emplace_back(std::make_shared<ServerDimension>(
		game, *this, identifier, this->dimensions.size(), std::move(mapGen)));
	
	DimensionPtr d = dimensions.back();
	return d;
}

DimensionPtr ServerWorld::getDimension(unsigned int index) {
	return dimensions[index];
}

DimensionPtr ServerWorld::getDimension(const std::string& identifier) {
	for (auto& dimension : dimensions)
		if (dimension->getIdentifier() == identifier) return dimension;
	throw std::runtime_error("No dimension named " + identifier + " found.");
}

InventoryRefsPtr ServerWorld::getRefs() {
	return InventoryRefsPtr(refs);
}

ServerClients& ServerWorld::getClients() {
	return clients;
}

void ServerWorld::changedMapBlocks(ServerPlayer& player) {
	generateMapBlocks(player);
	sendChunksToPlayer(player);
	player.changedMapBlocks = false;
}

void ServerWorld::generateMapBlocks(ServerPlayer& player) {
	unsigned int generating = 0;
	glm::ivec3 playerMapBlock = Space::MapBlock::world::fromBlock(player.getPos());

	for (const auto& c : generateOrder) {
		glm::ivec3 mapBlockPos = playerMapBlock + c;
		generating += generateMapBlock(player.getDim()->getInd(), mapBlockPos);
	}

	std::cout << "Player moved, generating " << generating << " MapBlocks." << std::endl;
}

bool ServerWorld::generateMapBlock(unsigned int dim, glm::ivec3 pos) {
	auto dimension = getDimension(dim);
	if (!dimension->getMapBlock(pos) || !dimension->getMapBlock(pos)->generated) return genStream->queue(dim, pos);
	return false;
}

void ServerWorld::sendChunksToPlayer(ServerPlayer& client) {
	ivec3 playerPos = Space::MapBlock::world::fromBlock(client.getPos());
	ivec3 lastPlayerPos = Space::MapBlock::world::fromBlock(client.lastPos);
	
	Bounds newBounds = { playerPos - ivec3{ sendRange.x, sendRange.y, sendRange.x },
		playerPos + ivec3{ sendRange.x, sendRange.y, sendRange.x }};
	Bounds oldBounds = { lastPlayerPos - ivec3{ sendRange.x, sendRange.y, sendRange.x },
		lastPlayerPos + ivec3{ sendRange.x, sendRange.y, sendRange.x }};
	
	for (auto& pos : generateOrder) {
		if (oldBounds.intersects(playerPos + pos) || !newBounds.intersects(playerPos + pos)) continue;
		packetStream->queue(client.getDim()->getInd(), pos + playerPos);
	}
}

void ServerWorld::sendMessage(const string& channel, const string& message) {
	auto p = Serializer().append(channel).append(message).packet(Packet::Type::MOD_MESSAGE);
	for (auto& client : clients.getClients())
		if (client.second->player)
			p.sendTo(client.second->player->getPeer(), Packet::Channel::ENTITY);
}
