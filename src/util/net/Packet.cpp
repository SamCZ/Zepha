//
// Created by aurailus on 10/01/19.
//

#include "Packet.h"

Packet::Packet(PacketType p) {
    this->type = p;
}

Packet::Packet(ENetPacket *packet) {
    std::string packetData(packet->data, packet->data + packet->dataLength);
    this->type = static_cast<PacketType>(Serializer::decodeInt(&packetData[0]));

    this->data = packetData.substr(4, packetData.length() - 4);
}

ENetPacket* Packet::toENetPacket() {
    std::string serialized;
    Serializer::encodeInt(serialized, static_cast<int>(type));
    serialized += data;

    ENetPacket* enet = enet_packet_create(serialized.data(), serialized.length() + 1, ENET_PACKET_FLAG_RELIABLE);
    return enet;
}

void Packet::sendTo(ENetPeer *peer, int channel) {
    ENetPacket* enet = toENetPacket();
    enet_peer_send(peer, (enet_uint8)channel, enet);
}

void Packet::sendTo(const ENetPeer &peer, int channel) {
    ENetPacket* enet = toENetPacket();
    enet_peer_send(const_cast<ENetPeer*>(&peer), (enet_uint8)channel, enet);
}
