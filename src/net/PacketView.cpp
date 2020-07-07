//
// Created by aurailus on 2020-03-07.
//

#include "PacketView.h"

PacketView::PacketView(ENetPacket *packet) :
    packet(packet, [](ENetPacket* p) { enet_packet_destroy(p); }),
    d(reinterpret_cast<char*>(this->packet->data), this->packet->dataLength),
    type(static_cast<PacketType>(d.read<unsigned int>())) {}
