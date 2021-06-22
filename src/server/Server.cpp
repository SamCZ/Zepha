//
// Created by aurailus on 09/01/19.
//

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
	const static i64 interval_ns = static_cast<long>((1000 / 60.f) * 1000000L);
	Timer loop("");
	
	world->update(delta);
	game.s()->update(delta);
	
	ENetEvent event;
	while (handler.update(&event) && loop.elapsedNs() < interval_ns) {
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
		
		case ENET_EVENT_TYPE_RECEIVE:
			packetReceived(event);
			break;
		}
	}
	
	for (u32 id : playersUpdated) {
		auto player = clients.getPlayer(id);
		if (!player) continue;
		
		Packet p = Serializer()
			.append(NetField::ID).append(static_cast<u32>(player->getId()))
			.append(NetField::POS).append(player->getPos())
			.append(NetField::LOOK_YAW).append(player->getYaw())
			.append(NetField::LOOK_PITCH).append(player->getPitch())
			.packet(Packet::Type::PLAYER_ENT_INFO, false);
		
		for (auto& iter : clients.players)
			if (iter->getId() != id && glm::distance(player->getPos(), iter->getPos()) < 200)
				p.sendTo(iter->getPeer(), Packet::Channel::ENTITY);
	}
	
	playersUpdated.clear();
	
	u64 sleep_for = interval_ns - loop.elapsedNs();
	if (sleep_for > 0) std::this_thread::sleep_for(std::chrono::nanoseconds(sleep_for));
	
	delta = loop.elapsedNs() / 1000000.f / 1000.f;
	elapsed += delta;
}

void Server::packetReceived(ENetEvent& e) {
	PacketView p(e.packet);
	auto client = static_cast<ServerClient*>(e.peer->data);
	auto player = PlayerPtr(clients.getPlayer(client->id));
	
	// Handle the packet as a player and then return.
	if (client->player) return playerPacketReceived(p, player);
	
	// Function returns true if a player is to be created.
	if (config.handlePacket(*client, p)) {
		auto clientShared = clients.getClient(client->id);
		if (clientShared) clients.createPlayer(clientShared, world->getDefaultDimension());
	}
}

void Server::playerPacketReceived(PacketView& p, PlayerPtr player) {
	// Pre-initialization because switch statements suck.
	ivec3 pos;
	u16 a, ind;
	string source, list;
	EVec face = EVec::NONE;
	
	switch (p.type) {
	default:
		throw std::runtime_error("Unknown packet type " +
			std::to_string(static_cast<u32>(p.type)) + " received.");
	
	case Packet::Type::THIS_PLAYER_INFO:
		player->handleAssertion(p.d);
		playersUpdated.emplace(player->getId());
		break;
	
	case Packet::Type::BLOCK_HIT:
		p.d.read(pos).read(face);
		player->getDim()->blockHit(Target(player->getDim(), pos, face), player);
		break;
	
	case Packet::Type::BLOCK_PLACE:
		p.d.read(pos).read(face);
		player->getDim()->blockPlace(Target(player->getDim(), pos, face), player);
		break;
	
	case Packet::Type::BLOCK_INTERACT:
		p.d.read(pos).read(face);
		player->getDim()->blockInteract(Target(player->getDim(), pos, face), player);
		break;
	
	case Packet::Type::BLOCK_PLACE_OR_INTERACT:
		p.d.read(pos).read(face);
		player->getDim()->blockPlaceOrInteract(Target(player->getDim(), pos, face), player);
		break;
	
	case Packet::Type::WIELD_ITEM_USE:
		player->getDim()->wieldItemUse(Target(), player);
		break;
	
	case Packet::Type::INV_WATCH:
		p.d.read<string>(source).read<string>(list);
		if (!world->getRefs().s()->addWatcher(source, list, player->getId()))
			Serializer().append(source).append(list).packet(Packet::Type::INV_INVALID)
				.sendTo(player.s()->getPeer(), Packet::Channel::INTERACT);
		break;
	
	case Packet::Type::INV_UNWATCH:
		p.d.read<string>(source).read<string>(list);
		if (!world->getRefs().s()->removeWatcher(source, list, player->getId()))
			Serializer().append(source).append(list).packet(Packet::Type::INV_INVALID)
				.sendTo(player.s()->getPeer(), Packet::Channel::INTERACT);
		break;
	
	case Packet::Type::INV_INTERACT:
		p.d.read<u16>(a).read<string>(source).read<string>(list).read<u16>(ind);
        world->getRefs().s()->interact(a, source, list, ind, player->getId());
		break;
		
	case Packet::Type::MOD_MESSAGE:
		p.d.read<string>(source).read<string>(list);
		game->getParser().safe_function(game->getParser().core["trigger"], "message",
			source, game->getParser().safe_function(game->getParser().core["deserialize"], list));
		break;
	}
}

void Server::cleanup() {
	alive = false;
}

Server::~Server() {
	cleanup();
}