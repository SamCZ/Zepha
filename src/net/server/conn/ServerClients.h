//
// Created by aurailus on 07/02/19.
//

#pragma once

#include <list>
#include <memory>
#include <enet/enet.h>

#include "../../../util/CovariantPtr.h"

class ServerWorld;
class ServerClient;
class ServerPlayer;
class ServerSubgame;
class ServerDimension;

class ServerClients {
public:
    explicit ServerClients(SubgamePtr game);
    void init(ServerWorld* world);

    void handleConnect(ENetEvent e);
    void handleDisconnect(ENetEvent e);

    void createPlayer(std::shared_ptr<ServerClient> client, DimensionPtr dimension);
    const std::shared_ptr<ServerClient> getClient(unsigned int id) const;
    const std::shared_ptr<ServerPlayer> getPlayer(unsigned int id) const;

    std::list<std::shared_ptr<ServerClient>> clients;
    std::list<std::shared_ptr<ServerPlayer>> players;

private:
    SubgamePtr game;
    ServerWorld* world;
};
