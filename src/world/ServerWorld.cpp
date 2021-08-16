#include <iostream>
#include <glm/glm.hpp>
#include <unordered_map>
#include <util/Timer.h>

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

ServerWorld::ServerWorld(u32 seed, SubgamePtr game, ServerClients& clients) :
	World(game),
	seed(seed),
	clients(clients),
	refs(make_shared<ServerInventoryRefs>(game, clients)) {
	clients.init(this);
	
	generateOrder.reserve(mapBlockGenRange.x * 2 + 1 * mapBlockGenRange.x * 2 + 1 * mapBlockGenRange.y * 2 + 1);
	std::unordered_set<ivec3, Vec::ivec3> found {};
	std::queue<ivec3> queue {};
	
	queue.emplace(0, 0, 0);
	found.emplace(0, 0, 0);
	
	while (!queue.empty()) {
		ivec3 pos = queue.front();
		queue.pop();
		
		generateOrder.push_back(pos);
		
		for (auto dir : Vec::TO_VEC) {
			ivec3 offset = pos + dir;
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

void ServerWorld::init(const string& worldDir) {
	genStream = make_unique<ServerGenStream>(*game.s(), *this);
//	packetStream = make_unique<ServerPacketStream>(*this);
//    fileManip = std::make_shared<FileManipulator>("worlds/" + worldDir + "/");
}

void ServerWorld::update(f64 delta) {
	for (auto& dimension : dimensions) dimension->update(delta);
	
	refs->update();
	
	u32 genCount = 0;
	
	auto finishedGen = genStream->update();
	std::unordered_set<ivec4, Vec::ivec4> chunksDirtied {};

	Timer t("Finishing Generation");
	for (auto& data : *finishedGen) {
		let dim = getDimension(data.dim);
		
		for (const auto& chunkPair : *data.created) {
			dim->setChunk(chunkPair.second);
			let chunkMapBlockPos = Space::MapBlock::world::fromChunk(chunkPair.first);
			if (chunkMapBlockPos != data.pos) {
				let chunkMapBlock = dim->getMapBlock(chunkMapBlockPos);
				if (chunkMapBlock->generated) chunksDirtied.insert(ivec4(chunkPair.first, data.dim));
			}
		}
		
		
		let mapBlock = dim->getMapBlock(ivec3(data.pos));
		assert(mapBlock);
		
		if (!mapBlock->generated) {
			mapBlock->generated = true;

			Serializer s {};
			for (u16 i = 0; i < 64; i++) {
				auto chunk = mapBlock->get(i);
				assert(chunk);
				if (chunk) s.append(chunk->compressToString());
			}
			
			let packet = s.packet(Packet::Type::MAPBLOCK);
			
			for (auto& client : clients.getClients()) {
				if (!client.second->player) continue;
				packet.sendTo(client.second->peer, Packet::Channel::WORLD);
			}
			
			genCount++;
			totalGens++;
		}
	}
	
	generatedMapBlocks = genCount;

	for (auto& chunkPos : chunksDirtied) {
        ivec3 mapBlockPos = Space::MapBlock::world::fromChunk(chunkPos);
        auto dim = getDimension(chunkPos.w);
        auto chunk = dim->getChunk(ivec3(chunkPos));
        assert(chunk);

        Packet p(Packet::Type::CHUNK);
        p.data = chunk->compressToString();

        for (auto& client : clients.getClients()) {
            if (!client.second->player) continue;
//            std::cout << "dirtied" << chunk->getPos() << std::endl;

//            auto myChunk = Space::Chunk::world::fromBlock(client.second->player->getPos());

//            std::pair<ivec3, ivec3> bounds = {
//                {myChunk.x - activeChunkRange.x, myChunk.y - activeChunkRange.y, myChunk.z - activeChunkRange.x},
//                {myChunk.x + activeChunkRange.x, myChunk.y + activeChunkRange.y, myChunk.z + activeChunkRange.x}};

//            if (isInBounds(chunkPos, bounds))
            p.sendTo(client.second->peer, Packet::Channel::WORLD);
        }
    }
	
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

DimensionPtr ServerWorld::createDimension(const string& identifier, std::unordered_set<string>& biomes) {
	dimensions.emplace_back(make_shared<ServerDimension>(
		game, *this, identifier, this->dimensions.size(),
		make_shared<MapGen>(**game, *this, seed, biomes)));
	
	dimensionIndexes[identifier] = dimensions.size() - 1;
	DimensionPtr d = dimensions.back();
	return d;
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
	u32 generating = 0;
	ivec3 playerMapBlock = Space::MapBlock::world::fromBlock(player.getPos());

	for (const auto& c : generateOrder) {
		ivec3 mapBlockPos = playerMapBlock + c;
		generating += generateMapBlock(player.getDim()->getInd(), mapBlockPos);
	}

//	std::cout << "Player moved, generating " << generating << " MapBlocks." << std::endl;
}

bool ServerWorld::generateMapBlock(u16 dim, ivec3 pos) {
	auto dimension = getDimension(dim);
	if (!dimension->getMapBlock(pos) || !dimension->getMapBlock(pos)->generated) return genStream->queue(dim, pos);
	return false;
}

void ServerWorld::sendChunksToPlayer(ServerPlayer& client) {
	ivec3 playerPos = Space::MapBlock::world::fromBlock(client.getPos());
	ivec3 lastPlayerPos = Space::MapBlock::world::fromBlock(client.lastPos);
	
	Bounds newBounds = { playerPos - ivec3 { sendRange.x, sendRange.y, sendRange.x },
		playerPos + ivec3 { sendRange.x, sendRange.y, sendRange.x }};
	Bounds oldBounds = { lastPlayerPos - ivec3 { sendRange.x, sendRange.y, sendRange.x },
		lastPlayerPos + ivec3 { sendRange.x, sendRange.y, sendRange.x }};
	
	for (auto& pos : generateOrder) {
		if (oldBounds.intersects(playerPos + pos) || !newBounds.intersects(playerPos + pos)) continue;
		
		auto dim = client.getDim();
		auto mb = dim->getMapBlock(playerPos + pos);
		if (!mb) return;
//		std::cout << "sending " << pos << " to " << client.getId() << std::endl;
		Serializer s {};
		for (u16 i = 0; i < 64; i++) {
			auto chunk = mb->get(i);
			if (chunk) s.append(chunk->compressToString());
		}
		let packet = s.packet(Packet::Type::MAPBLOCK);

		for (auto& client : clients.getClients()) {
			if (!client.second->player) continue;
			packet.sendTo(client.second->peer, Packet::Channel::WORLD);
		}
	}
}

void ServerWorld::sendMessage(const string& channel, const string& message) {
	auto p = Serializer().append(channel).append(message).packet(Packet::Type::MOD_MESSAGE);
	for (auto& client : clients.getClients())
		if (client.second->player)
			p.sendTo(client.second->player->getPeer(), Packet::Channel::ENTITY);
}
