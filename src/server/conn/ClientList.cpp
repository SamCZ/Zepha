//
// Created by aurailus on 07/02/19.
//

#include "ClientList.h"
#include "../../util/net/Serializer.h"
#include "../../util/net/NetHandler.h"
#include "../../def/ServerGame.h"


ClientList::ClientList(ServerGame& defs) :
    defs(defs) {}

void ClientList::handleConnect(ENetEvent e) {
    ENetPeer* peer = e.peer;
    ENetAddress& addr = peer->address;

    auto client = std::make_shared<ServerClient>(peer, addr, defs.defs);
    clients.push_back(client);
    peer->data = client.get();

    std::cout << Log::info << NetHandler::intToIPString(addr.host) << ":" << addr.port << " connected." << Log::endl;
}

void ClientList::handleDisconnect(ENetEvent e) {
    unsigned int cid = static_cast<ServerClient*>(e.peer->data)->cid;

    for (unsigned int i = 0; i < clients.size(); i++) {
        if (clients[i]->cid == cid) {
            defs.parser.playerDisconnected(clients[i]);
            clients.erase(clients.begin() + i);
            break;
        }
    }

    ENetAddress& addr = e.peer->address;
    std::cout << Log::info << NetHandler::intToIPString(addr.host) << ":" << addr.port << " disconnected." << Log::endl;
}

void ClientList::createPlayer(std::shared_ptr<ServerClient> c) {
    c->hasPlayer = true;
    c->setUsername("TEMPORaRY");

    defs.parser.playerConnected(c);

    Packet p(PacketType::THIS_PLAYER_INFO);
    p.data = Serializer()
            .append(c->cid)
            .append(c->getPos())
            .append(c->getPitch())
            .append(c->getYaw())
            .data;

    p.sendTo(c->peer, PacketChannel::ENTITY);
}