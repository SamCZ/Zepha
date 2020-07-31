//
// Created by aurailus on 07/02/19.
//

#include <iostream>

#include "ServerClients.h"

#include "ServerPlayer.h"
#include "ServerClient.h"
#include "../../Serializer.h"
#include "../../NetHandler.h"
#include "../../../util/Log.h"
#include "../../../def/ServerSubgame.h"
#include "../../../lua/ServerLuaParser.h"

ServerClients::ServerClients(ServerSubgame& game) :
    game(game) {}

void ServerClients::init(ServerWorld *world) {
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
            game.getParser().playerDisconnected(*it);
            players.erase(it);
            break;
        }
    }

    for (auto it = clients.begin(); it != clients.end();) {
        if ((*it)->id == id) { clients.erase(it); break; }
    }
}

void ServerClients::createPlayer(std::shared_ptr<ServerClient> client, ServerDimension& dimension) {
    auto player = std::make_shared<ServerPlayer>(*client, game, dimension);
    player->getInventory().createList("cursor", 1, 1);
    client->player = player;

    players.push_back(player);
    game.getParser().playerConnected(player);

    Serializer()
        .appendE(Player::NetField::ID).append(player->getId())
        .appendE(Player::NetField::POS).append(player->getPos())
        .appendE(Player::NetField::PITCH).append(player->getPitch())
        .appendE(Player::NetField::YAW).append(player->getYaw())
        .packet(PacketType::THIS_PLAYER_INFO).sendTo(player->getPeer(), PacketChannel::ENTITY);
}

const std::shared_ptr<ServerClient> ServerClients::getClient(unsigned int id) const {
    for (auto& c : clients) if (c->id == id) return c;
    return nullptr;
}


const std::shared_ptr<ServerPlayer> ServerClients::getPlayer(unsigned int id) const {
    for (auto& c : players) if (c->getId() == id) return c;
    return nullptr;
}
