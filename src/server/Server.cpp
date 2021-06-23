#include <thread>
#include <iostream>

#include "Server.h"

#include "util/Timer.h"
#include "util/Target.h"
#include "util/net/NetField.h"
#include "game/def/BlockDef.h"
#include "util/net/Serializer.h"
#include "server/ServerClient.h"
#include "lua/usertype/Player.h"
#include "world/player/ServerPlayer.h"

Server::Server(u16 port, const std::string& subgame) :
	seed(69),
	port(port),
	config(game),
	clients(game),
	handler(port, 32),
	game(make_shared<ServerSubgame>(subgame, seed)),
	world(make_shared<ServerWorld>(seed, game, clients)) {
	
	game.s()->init(world);
	world.s()->init("world");
	config.init();
	
	std::cout << Log::info << "Server started successfully, listening for clients." << Log::endl;
	while (alive) update();
}

void Server::update() {
	const static i64 interval_ns = static_cast<i64>((1000 / 60.f) * 1000000L);
	Timer loop("");
	
	world->update(delta);
	game.s()->update(delta);
	
	// Read incoming events.
	
	ENetEvent event;
	u64 runs = 0;
	while (handler.update(&event) && (loop.elapsedNs() < interval_ns || runs++ < 8)) {
		switch (event.type) {
		default:
		case ENET_EVENT_TYPE_NONE:
			throw std::runtime_error("Unknown packet type " + std::to_string(event.type) + " received.");
		
		case ENET_EVENT_TYPE_CONNECT:
			clients.handleConnect(event);
			break;
		
		case ENET_EVENT_TYPE_DISCONNECT:
			clients.handleDisconnect(event);
			break;
		
		case ENET_EVENT_TYPE_RECEIVE: {
			PacketView p(event.packet);
			
			auto client = static_cast<ServerClient*>(event.peer->data);
			if (client->player) return playerPacketReceived(p, clients.getClient(client->id)->player);
			
			if (config.handlePacket(*client, p)) {
				auto clientShr = clients.getClient(client->id);
				clients.createPlayer(clientShr, world->getDefaultDimension());
			}
			
			break;
		}
		}
	}
	
	// Update players with the status of other players.
	
	for (u32 id : playersUpdated) {
		auto client = clients.getClient(id);
		if (!client) continue;
		auto player = client->player;
		if (!player) continue;
		
		Packet p = Serializer()
			.append(NetField::ID).append(static_cast<u32>(player->getId()))
			.append(NetField::POS).append(player->getPos())
			.append(NetField::LOOK_YAW).append(player->getYaw())
			.append(NetField::LOOK_PITCH).append(player->getPitch())
			.packet(Packet::Type::PLAYER_ENT_INFO, false);
		
		for (auto& iter : clients.getClients())
			if (iter.first != id && iter.second->player &&
				glm::distance(player->getPos(), iter.second->player->getPos()) < 200)
				p.sendTo(iter.second->peer, Packet::Channel::ENTITY);
	}
	
	playersUpdated.clear();
	
	u64 sleep_for = (std::max)(interval_ns - static_cast<i64>(loop.elapsedNs()), 0L);
	if (sleep_for > 0) std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_for));
	
	delta = loop.elapsedNs() / 1000000.f / 1000.f;
	elapsed += delta;
}

void Server::playerPacketReceived(PacketView& p, sptr<ServerPlayer> player) {
	switch (p.type) {
	default: {
		throw std::runtime_error("Unknown packet type: " + std::to_string(static_cast<u32>(p.type)));
	}
	
	case Packet::Type::THIS_PLAYER_INFO: {
		player->handleAssertion(p.d);
		playersUpdated.emplace(player->getId());
		break;
	}
	
	case Packet::Type::BLOCK_HIT: {
		ivec3 pos = p.d.read<ivec3>();
		EVec face = p.d.read<EVec>();
		player->getDim()->blockHit(Target(player->getDim(), pos, face), player);
		break;
	}
	
	case Packet::Type::BLOCK_PLACE: {
		ivec3 pos = p.d.read<ivec3>();
		EVec face = p.d.read<EVec>();
		player->getDim()->blockPlace(Target(player->getDim(), pos, face), player);
		break;
	}
	
	case Packet::Type::BLOCK_INTERACT: {
		ivec3 pos = p.d.read<ivec3>();
		EVec face = p.d.read<EVec>();
		player->getDim()->blockInteract(Target(player->getDim(), pos, face), player);
		break;
	}
	
	case Packet::Type::BLOCK_PLACE_OR_INTERACT: {
		ivec3 pos = p.d.read<ivec3>();
		EVec face = p.d.read<EVec>();
		player->getDim()->blockPlaceOrInteract(Target(player->getDim(), pos, face), player);
		break;
	}
	
	case Packet::Type::WIELD_ITEM_USE: {
		player->getDim()->wieldItemUse(Target(), player);
		break;
	}
	
	case Packet::Type::INV_WATCH: {
		string source = p.d.read<string>();
		string list = p.d.read<string>();
		if (!world->getRefs().s()->addWatcher(source, list, player->getId()))
			Serializer().append(source).append(list).packet(Packet::Type::INV_INVALID)
				.sendTo(player->getPeer(), Packet::Channel::INTERACT);
		break;
	}
	
	case Packet::Type::INV_UNWATCH: {
		string source = p.d.read<string>();
		string list = p.d.read<string>();
		if (!world->getRefs().s()->removeWatcher(source, list, player->getId()))
			Serializer().append(source).append(list).packet(Packet::Type::INV_INVALID)
				.sendTo(player->getPeer(), Packet::Channel::INTERACT);
		break;
	}
	
	case Packet::Type::INV_INTERACT: {
		bool primary = p.d.read<bool>();
		string source = p.d.read<string>();
		string list = p.d.read<string>();
		u16 ind = p.d.read<u16>();
		world->getRefs().s()->interact(primary, source, list, ind, player->getId());
		break;
	}
	
	case Packet::Type::MOD_MESSAGE: {
		string source = p.d.read<string>();
		string list = p.d.read<string>();
		game->getParser().safe_function(game->getParser().core["trigger"], "message",
			source, game->getParser().safe_function(game->getParser().core["deserialize"], list));
		break;
	}
	}
}

void Server::cleanup() {
	alive = false;
}

Server::~Server() {
	cleanup();
}