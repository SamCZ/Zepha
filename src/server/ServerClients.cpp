//
// Created by aurailus on 07/02/19.
//

#include <iostream>

#include "ServerClients.h"

#include "util/Log.h"
#include "ServerClient.h"
#include "util/net/NetField.h"
#include "util/net/Serializer.h"
#include "util/net/NetHandler.h"
#include "world/player/ServerPlayer.h"

ServerClients::ServerClients(SubgamePtr game) :
	game(game) {}

void ServerClients::init(ServerWorld* world) {
	this->world = world;
}

void ServerClients::handleConnect(ENetEvent e) {
	auto client = std::make_shared<ServerClient>("TEMPORARY", e.peer);
	client->peer->data = client.get();
	clients.push_back(client);
	
	std::cout << Log::info << NetHandler::intToIPString(client->address.host)
	          << ":" << client->address.port << " connected." << Log::endl;
}

void ServerClients::handleDisconnect(ENetEvent e) {
	unsigned int id = static_cast<ServerClient*>(e.peer->data)->id;
	
	ENetAddress& addr = e.peer->address;
	std::cout << Log::info << NetHandler::intToIPString(addr.host) << ":" << addr.port << " disconnected." << Log::endl;
	
	// Disconnect the player, if it exists.
	for (auto it = players.begin(); it != players.end();) {
		if ((*it)->getId() == id) {
			game.s()->getParser().playerDisconnected(*it);
			players.erase(it);
			break;
		}
	}
	
	for (auto it = clients.begin(); it != clients.end();) {
		if ((*it)->id == id) {
			clients.erase(it);
			break;
		}
	}
}

void ServerClients::createPlayer(std::shared_ptr<ServerClient> client, DimensionPtr dimension) {
	auto player = std::make_shared<ServerPlayer>(*client, dimension->getWorld(), game, dimension);
	player->getInventory()->createList("cursor", 1, 1);
	client->player = player;
	players.push_back(player);
	game.s()->getParser().playerConnected(player);
	
	player->setPos({ 32, -20, 32 }, true);
	
	Serializer()
		.append(NetField::ID).append(player->getId())
		.append(NetField::POS).append(player->getPos())
		.append(NetField::LOOK_PITCH).append(player->getPitch())
		.append(NetField::LOOK_YAW).append(player->getYaw())
		.packet(Packet::Type::THIS_PLAYER_INFO).sendTo(player->getPeer(), Packet::Channel::INTERACT);
}

const std::shared_ptr<ServerClient> ServerClients::getClient(unsigned int id) const {
	for (auto& c : clients) if (c->id == id) return c;
	return nullptr;
}


const std::shared_ptr<ServerPlayer> ServerClients::getPlayer(unsigned int id) const {
	for (auto& c : players) if (c->getId() == id) return c;
	return nullptr;
}
