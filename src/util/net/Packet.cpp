#include "Packet.h"

#include "Serializer.h"

Packet::Packet(Type type, bool reliable) : type(type), reliable(reliable) {}

ENetPacket* Packet::toENetPacket() const {
	string serialized = Serializer().append(static_cast<u8>(type)).data + data;
	
	ENetPacket* enet = enet_packet_create(serialized.data(), serialized.length(),
		(reliable ? ENET_PACKET_FLAG_RELIABLE : 0));
	return enet;
}

void Packet::sendTo(ENetPeer* peer, Channel channel) const {
	ENetPacket* enet = toENetPacket();
	enet_peer_send(peer, static_cast<enet_uint8>(channel), enet);
}

void Packet::sendTo(const ENetPeer& peer, Channel channel) const {
	ENetPacket* enet = toENetPacket();
	enet_peer_send(const_cast<ENetPeer*>(&peer), static_cast<enet_uint8>(channel), enet);
}