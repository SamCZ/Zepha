//
// Created by aurailus on 10/01/19.
//

#include "Packet.h"

Packet::Packet(PacketType type) : type(type) {}

Packet::Packet(ENetPacket *packet) {
    std::string packetData(packet->data, packet->data + packet->dataLength);
    this->type = static_cast<PacketType>(OLDSerializer::decodeInt(&packetData[0]));

    this->data = packetData.substr(4, packetData.length() - 4);
}

ENetPacket* Packet::toENetPacket() {
    std::string serialized = Serializer().append(static_cast<int>(type)).data + data;

    ENetPacket* enet = enet_packet_create(serialized.data(), serialized.length() + 1, ENET_PACKET_FLAG_RELIABLE);
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
