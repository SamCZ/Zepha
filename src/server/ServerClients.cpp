#include <iostream>

#include "ServerClients.h"

#include "util/Log.h"
#include "ServerClient.h"
#include "util/net/NetField.h"
#include "util/net/Serializer.h"
#include "util/net/NetHandler.h"
#include "world/player/ServerPlayer.h"

ServerClients::ServerClients(SubgamePtr game) : game(game) {}

void ServerClients::init(ServerWorld* world) {
	this->world = world;
}

void ServerClients::handleConnect(ENetEvent e) {
	const auto client = make_shared<ServerClient>("TEMP:" + NetHandler::intToIPString(*reinterpret_cast<unsigned int*>(&e.peer->address.host))
		+ ":" + std::to_string(e.peer->address.port), e.peer->connectID, e.peer);
	e.peer->data = client.get();
	clients.emplace(client->id, client);
	
	std::cout << Log::info << client->username << " connected." << Log::endl;
}

void ServerClients::handleDisconnect(ENetEvent e) {
	const auto client = static_cast<ServerClient*>(e.peer->data);
	if (client->player) game.s()->getParser().playerDisconnected(client->player);
	
	std::cout << Log::info << client->username << " disconnected." << Log::endl;
	clients.erase(client->id);
}

void ServerClients::createPlayer(sptr<ServerClient> client, DimensionPtr dimension) {
	client->player = make_shared<ServerPlayer>(*client, dimension->getWorld(), game, dimension);
	
	game.s()->getParser().playerConnected(client->player);
//	client->player->setPos({ 256, -20, 256 }, true);
	
	Serializer()
		.append(NetField::ID).append(static_cast<u32>(client->player->getId()))
		.append(NetField::POS).append(client->player->getPos())
		.append(NetField::LOOK_PITCH).append(client->player->getPitch())
		.append(NetField::LOOK_YAW).append(client->player->getYaw())
		.packet(Packet::Type::THIS_PLAYER_INFO).sendTo(client->peer, Packet::Channel::INTERACT);
}

const sptr<ServerClient> ServerClients::getClient(u32 id) const {
	if (clients.count(id)) return clients.at(id);
	return nullptr;
}

const std::unordered_map<u32, sptr<ServerClient>>& ServerClients::getClients() const {
	return clients;
}