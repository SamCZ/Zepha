//
// Created by aurailus on 10/01/19.
//

#include "Packet.h"

Packet::Packet() {
    this->type = UNDEFINED;
}

Packet::Packet(Packet::p_type p) {
    this->type = p;
}

Packet::Packet(ENetPacket *packet) {
    std::string packetData(packet->data, packet->data + packet->dataLength);
    this->type = Serializer::decodeInt(&packetData[0]);

    this->data = packetData.substr(4, packetData.length() - 4);
}

ENetPacket *Packet::toENetPacket() {
    std::string serialized;
    Serializer::encodeInt(serialized, (int)this->type);
    serialized += this->data;

    ENetPacket* enet = enet_packet_create(serialized.data(), serialized.length() + 1, ENET_PACKET_FLAG_RELIABLE);
    return enet;
}
