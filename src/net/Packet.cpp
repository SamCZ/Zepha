//
// Created by aurailus on 10/01/19.
//

#include "Packet.h"

#include "Serializer.h"
#include "Deserializer.h"

Packet::Packet(PacketType type, bool reliable) : type(type), reliable(reliable) {}

ENetPacket* Packet::toENetPacket() {
    std::string serialized = Serializer().append(static_cast<unsigned int>(type)).data + data;

    ENetPacket* enet = enet_packet_create(serialized.data(), serialized.length(), (reliable ? ENET_PACKET_FLAG_RELIABLE : 0));
    return enet;
}

void Packet::sendTo(ENetPeer *peer, PacketChannel channel) {
    ENetPacket* enet = toENetPacket();
    enet_peer_send(peer, static_cast<enet_uint8>(channel), enet);
}

void Packet::sendTo(const ENetPeer &peer, PacketChannel channel) {
    ENetPacket* enet = toENetPacket();
    enet_peer_send(const_cast<ENetPeer*>(&peer), static_cast<enet_uint8>(channel), enet);
}