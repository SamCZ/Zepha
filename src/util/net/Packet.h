//
// Packet implentation for easy manipulation.
// Can be converted into an ENet packet, or deserialized from one.
// Created by aurailus on 10/01/19.
//

#pragma once

#include <string>
#include <enet/enet.h>

class Packet {
	public:
	enum class Channel {
		UNDEFINED = 0,
		// Authentication
		AUTH, CONNECT,
		// Misc Server Information
		SERVER,
		// Data streams
		WORLD, ENTITY, INTERACT
	};
	
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
	
	explicit Packet(Type type, bool reliable = true);
	
	ENetPacket* toENetPacket() const;
	
	void sendTo(ENetPeer* peer, Channel channel) const;
	
	void sendTo(const ENetPeer& peer, Channel channel) const;
	
	Type type = Type::UNDEFINED;
	std::string data{};
	bool reliable = true;
};