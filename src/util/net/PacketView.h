#pragma once

#include <enet/enet.h>

#include "Packet.h"
#include "util/Types.h"
#include "Deserializer.h"

/**
 * Provides a read-only view into a packet.
 * Manages the internal ENetPacket's memory, only deleting it when the PacketView is destroyed.
 * Initializes a deserializer instance with the packet's data.
 */

class PacketView {
public:
	PacketView(const PacketView&) = delete;
	
	explicit PacketView(ENetPacket* packet);
	
	/** A deserializer to read the packet's data. */
	Deserializer d;
	
	/** The packet's type. */
	Packet::Type type;
	
private:
	/** An internal reference to the raw packet. */
	sptr<ENetPacket> packet;
};
