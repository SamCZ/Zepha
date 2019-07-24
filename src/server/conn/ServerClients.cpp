//
// Created by aurailus on 07/02/19.
//

#include "ServerClients.h"

void ServerClients::handleConnect(ENetEvent e) {
    ENetPeer* peer = e.peer;
    ENetAddress& addr = peer->address;

    std::cout << Log::info << addr.host << ":" << addr.port << " connected." << Log::endl;

    clients.emplace_back(peer, addr);
    ServerClient& client = clients.back();

    //TODO: Create this later
    peer->data = &client;
}

void ServerClients::handleDisconnect(ENetEvent e) {
    ENetPeer* peer = e.peer;
    ENetAddress& addr = peer->address;

    unsigned int connectID = static_cast<ServerClient*>(peer->data)->getConnectID();

    std::cout << Log::info << addr.host << ":" << addr.port << " disconnected." << Log::endl;

    bool found = false;
    auto it = clients.begin();
    while (it != clients.end()) {
        if ((*it).getConnectID() == connectID) {
            found = true;
            clients.erase(it);
            break;
        }
        it ++;
    }

    if (!found) std::cout << Log::err << "Tried to disconnect nonexistent client!" << Log::endl;
}

void ServerClients::createPlayer(ServerClient &c) {
    c.initPlayer();

    Packet p(PacketType::PLAYER);
    Serializer::encodeInt(p.data, c.getConnectID());
    Serializer::encodeFloatVec3(p.data, c.getPlayer().getPos());
    p.sendTo(c.getPeer(), PacketChannel::PLAYER);
}
