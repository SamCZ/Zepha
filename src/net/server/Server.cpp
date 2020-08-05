//
// Created by aurailus on 09/01/19.
//

#include <thread>
#include <iostream>

#include "Server.h"

#include "../Serializer.h"
#include "../../util/Timer.h"
#include "conn/ServerClient.h"
#include "conn/ServerPlayer.h"
#include "../../world/Target.h"
#include "../../def/item/BlockDef.h"
#include "../../game/scene/world/NetField.h"

Server::Server(unsigned short port, const std::string& subgame) :
    seed(69),
    port(port),
    config(game),
    clients(game),
    handler(port, 32),
    game(std::make_shared<ServerSubgame>(subgame, seed)),
    world(std::make_shared<ServerWorld>(seed, game, clients)) {

    game.s()->init(world);
    world.s()->init("world");
    config.init();

    std::cout << Log::info << "Server started successfully, listening for clients." << Log::endl;
    while (alive) update();
}

void Server::update() {
    const static long interval_ns = static_cast<long>((1000 / 60.f) * 1000000L);
    Timer loop("");

    world->update(delta);
    game.s()->update(delta);

    ENetEvent event;
    while (handler.update(&event) && loop.elapsedNs() < interval_ns) {
        switch (event.type) {
            default:
            case ENET_EVENT_TYPE_NONE:
                throw std::runtime_error("Unknown packet type " + std::to_string(event.type) + " received."); break;

            case ENET_EVENT_TYPE_CONNECT:
                clients.handleConnect(event); break;

            case ENET_EVENT_TYPE_DISCONNECT:
                clients.handleDisconnect(event); break;

            case ENET_EVENT_TYPE_RECEIVE:
                packetReceived(event); break;
        }
    }

    for (auto& id : playersUpdated) {
        auto player = clients.getPlayer(id);
        if (!player) continue;

        Packet p = Serializer()
            .appendE(NetField::ID).append(player->getId())
            .appendE(NetField::POS).append(player->getPos())
            .appendE(NetField::LOOK_YAW).append(player->getYaw())
            .appendE(NetField::LOOK_PITCH).append(player->getPitch())
            .packet(Packet::Type::PLAYER_ENT_INFO, false);

        for (auto& iter : clients.players)
            if (iter->getId() != id && glm::distance(player->getPos(), iter->getPos()) < 200)
                p.sendTo(iter->getPeer(), Packet::Channel::ENTITY);
    }

    playersUpdated.clear();

    long sleep_for = interval_ns - loop.elapsedNs();
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
    glm::ivec3 pos;
    EVec face = EVec::NONE;
    std::string source, list;
    unsigned short a, ind;

    switch (p.type) {
        default:
            throw std::runtime_error("Unknown packet type " + std::to_string(static_cast<int>(p.type)) + " received."); break;

        case Packet::Type::THIS_PLAYER_INFO:
            player->handleAssertion(p.d);
            playersUpdated.emplace(player->getId()); break;

//        case Packet::Type::BLOCK_SET:
//            glm::ivec3 pos = p.d.read<glm::ivec3>();
//            unsigned int block = p.d.read<unsigned int>();
//
//            unsigned int worldBlock = (block == DefinitionAtlas::AIR ? world.getBlock(pos) : 0);
//
//            if (block == DefinitionAtlas::AIR) {
//                auto& def = game.defs->blockFromId(worldBlock);
//                if (def.callbacks.count(Callback::BREAK)) game.lua->safe_function(def.callbacks[Callback::BREAK], pos, ServerLuaPlayer(client));
//                game.lua->safe_function(game.lua->core["trigger"], "break", pos, ServerLuaPlayer(client));
//            }
//            else {
//                auto& def = game.defs->blockFromId(block);
//                if (def.callbacks.count(Callback::PLACE)) game.lua->safe_function(def.callbacks[Callback::PLACE], pos, ServerLuaPlayer(client));
//                game.lua->safe_function(game.lua->core["trigger"], "place", pos, ServerLuaPlayer(client));
//            }
//
//            world.setBlock(pos, block);
//
//            if (block == DefinitionAtlas::AIR) {
//                auto& def = game.defs->blockFromId(worldBlock);
//                if (def.callbacks.count(Callback::AFTER_BREAK))
//                    game.lua->safe_function(def.callbacks[Callback::AFTER_BREAK], pos, ServerLuaPlayer(client));
//                game.lua->safe_function(game.lua->core["trigger"], "after_break", pos, ServerLuaPlayer(client));
//            }
//            else {
//                auto& def = game.defs->blockFromId(block);
//                if (def.callbacks.count(Callback::AFTER_PLACE))
//                    game.lua->safe_function(def.callbacks[Callback::AFTER_PLACE], pos, ServerLuaPlayer(client));
//                game.lua->safe_function(game.lua->core["trigger"], "after_place", pos, ServerLuaPlayer(client));
//            }
//            break; }

        case Packet::Type::BLOCK_HIT:
            p.d.read(pos).readE(face);
            player->getDim()->blockHit(Target(player->getDim(), pos, face), player); break;

        case Packet::Type::BLOCK_PLACE:
            p.d.read(pos).readE(face);
            player->getDim()->blockPlace(Target(player->getDim(), pos, face), player); break;

        case Packet::Type::BLOCK_INTERACT:
            p.d.read(pos).readE(face);
            player->getDim()->blockInteract(Target(player->getDim(), pos, face), player); break;

        case Packet::Type::BLOCK_PLACE_OR_INTERACT:
            p.d.read(pos).readE(face);
            player->getDim()->blockPlaceOrInteract(Target(player->getDim(), pos, face), player); break;

        case Packet::Type::INV_WATCH:
            p.d.read<std::string>(source).read<std::string>(list);
            if (!world->getRefs().s()->addWatcher(source, list, player->getId()))
                Serializer().append(source).append(list).packet(Packet::Type::INV_INVALID)
                    .sendTo(player.s()->getPeer(), Packet::Channel::INTERACT); break;

        case Packet::Type::INV_UNWATCH:
            p.d.read<std::string>(source).read<std::string>(list);
            if (!world->getRefs().s()->removeWatcher(source, list, player->getId()))
                Serializer().append(source).append(list).packet(Packet::Type::INV_INVALID)
                    .sendTo(player.s()->getPeer(), Packet::Channel::INTERACT); break;

        case Packet::Type::INV_INTERACT:
            p.d.read<unsigned short>(a).read<std::string>(source).read<std::string>(list).read<unsigned short>(ind);
            world->getRefs().s()->interact(a, source, list, ind, player->getId()); break;
    }
}

void Server::cleanup() {
    alive = false;
}

Server::~Server() {
    cleanup();
}