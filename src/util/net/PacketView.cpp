#include "PacketView.h"

#include "Packet.h"

PacketView::PacketView(ENetPacket* packet) :
	packet(packet, [](ENetPacket* p) { enet_packet_destroy(p); }),
	d(reinterpret_cast<char*>(packet->data), packet->dataLength),
	type(static_cast<Packet::Type>(d.read<u8>())) {}
