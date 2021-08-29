#pragma once

#include <enet.h>

#include "util/Types.h"

/**
 * A packet stores data to send across a connection,
 * allowing data transmission between the client and server.
 * This class is used to create a packet,
 * if the ability to read a sent packet is needed, use PacketView.
 */

class Packet {
public:
	/** An enum of packet channels that can be used. */
	enum class Channel {
		UNDEFINED = 0,
		// Authentication
		AUTH, CONNECT,
		// Misc Server Information
		SERVER,
		// Data streams
		WORLD, ENTITY, INTERACT
	};
	
	/** An enum of packet types that can be used. */
	enum class Type {
		UNDEFINED = 0,
		// Information Request Types
		BLOCK_IDENTIFIER_LIST, BIOME_IDENTIFIER_LIST, MOD_ORDER,
		MODS, MEDIA, MEDIA_DONE, CONNECT_DATA_RECVD,
		// Miscellaneous
		SERVER_INFO, MOD_MESSAGE,
		// Player Info
		PLAYER_ENT_INFO, THIS_PLAYER_INFO,
		// World
		CHUNK, MAPBLOCK,
		// Block
		BLOCK_HIT, BLOCK_PLACE, BLOCK_INTERACT,
		BLOCK_PLACE_OR_INTERACT, BLOCK_SET,
		// Item
		WIELD_ITEM_USE,
		// Entity
		ENTITY_INFO, ENTITY_REMOVED,
		// Inventory
		INV_WATCH, INV_UNWATCH, INV_INVALID, INV_DATA, INV_INTERACT
	};
	
	Packet() = default;
	
	/** Creates a packet with the type specified. */
	explicit Packet(Type type, bool reliable = true);
	
	/** Converts the packet into a raw packet to transfer. */
	ENetPacket* toENetPacket() const;

	/** Sends the packet to the peer specified. */
	void sendTo(ENetPeer* peer, Channel channel) const;
	
	/** Sends the packet to the peer specified. */
	void sendTo(const ENetPeer& peer, Channel channel) const;

	/** The internal packet data. */
	string data {};
	
	/** Whether or not the packet should be reliably sequenced. */
	bool reliable = true;
	
	/** The packet's type. */
	Type type = Type::UNDEFINED;
};