//
// Created by aurailus on 03/07/19.
//

#include "ServerInfoSender.h"

#include "game/def/ItemDef.h"
#include "game/def/BiomeDef.h"
#include "game/ServerSubgame.h"
#include "util/net/PacketView.h"
#include "util/net/Serializer.h"
#include "server/ServerClient.h"
#include "game/atlas/asset/AssetType.h"

ServerInfoSender::ServerInfoSender(SubgamePtr game) : game(game) {}

void ServerInfoSender::init() {
	blockIdentifierList.reserve(game->getDefs().size());
	for (usize i = 0; i < game->getDefs().size(); i++)
		blockIdentifierList.push_back(game->getDefs().fromId(i).identifier);
	
	biomeIdentifierList.reserve( game->getBiomes().size());
	for (usize i = 0; i < game->getBiomes().size(); i++)
		biomeIdentifierList.push_back(game->getBiomes().biomeFromId(i).identifier);
}

bool ServerInfoSender::handlePacket(ServerClient& client, PacketView& r) {
	switch (r.type) {
	default:
		break;
	
	case Packet::Type::CONNECT_DATA_RECVD:
		return true;
	
	case Packet::Type::SERVER_INFO:
		Serializer()
			.append(game.s()->getBiomes().getSeed())
			.packet(Packet::Type::SERVER_INFO)
			.sendTo(client.peer, Packet::Channel::CONNECT);
		break;
	
	case Packet::Type::BLOCK_IDENTIFIER_LIST:
		Serializer()
			.append(blockIdentifierList)
			.packet(Packet::Type::BLOCK_IDENTIFIER_LIST)
			.sendTo(client.peer, Packet::Channel::CONNECT);
		break;
	
	case Packet::Type::BIOME_IDENTIFIER_LIST:
		Serializer()
			.append(biomeIdentifierList)
			.packet(Packet::Type::BIOME_IDENTIFIER_LIST)
			.sendTo(client.peer, Packet::Channel::CONNECT);
		break;
	
	case Packet::Type::MODS: {
		game.s()->getParser().sendModsPacket(client.peer);
		break;
	}
	
	case Packet::Type::MEDIA: {
		const static usize MAX_PACKET_SIZE = 32 * 1024;
		usize packetSize = 0;
		
		Serializer s {};
		
		for (ServerTexture& texture : game.s()->assets.textures) {
			if (packetSize + 20 + texture.data.length() > MAX_PACKET_SIZE && packetSize != 0) {
				s.append(AssetType::END);
				Packet p(Packet::Type::MEDIA);
				
				p.data = s.data;
				packetSize = 0;
				s = {};
				
				p.sendTo(client.peer, Packet::Channel::CONNECT);
			}
			
			s.append(AssetType::TEXTURE)
			 .append(texture.name)
			 .append(texture.width)
			 .append(texture.height)
			 .append(texture.data);
			
			packetSize += texture.data.length() + 20;
		}
		
		for (SerializedModel& model : game.s()->assets.models) {
			if (packetSize + 16 + model.data.length() > MAX_PACKET_SIZE && packetSize != 0) {
				s.append(AssetType::END);
				Packet p(Packet::Type::MEDIA);
				
				p.data = s.data;
				packetSize = 0;
				s = {};
				
				p.sendTo(client.peer, Packet::Channel::CONNECT);
			}
			
			s.append(AssetType::MODEL)
			 .append(model.name)
			 .append(model.format)
			 .append(model.data);
			
			packetSize += model.data.length() + 16;
		}
		
		s.append(AssetType::END);
		Packet p(Packet::Type::MEDIA);
		p.data = s.data;
		
		p.sendTo(client.peer, Packet::Channel::CONNECT);
		
		Packet d(Packet::Type::MEDIA_DONE);
		d.sendTo(client.peer, Packet::Channel::CONNECT);
	}
	}
	
	return false;
}
