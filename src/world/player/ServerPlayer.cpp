#include "ServerPlayer.h"

#include "world/ServerWorld.h"
#include "util/net/NetField.h"
#include "server/ServerClient.h"
#include "util/net/Serializer.h"
#include "util/net/Deserializer.h"
#include "world/inv/ServerInventoryRefs.h"

ServerPlayer::ServerPlayer(ServerClient& client, World& world, SubgamePtr game, DimensionPtr dim) :
	Player(game, world, dim, client.id), Entity(game, dim), client(client),
	inventory(world.getRefs()->createInventory("player:" + std::to_string(id))) {
	
	inventory->createList("cursor", 1, 1);
}

void ServerPlayer::assertField(Packet packet) {
	packet.type = Packet::Type::THIS_PLAYER_INFO;
	packet.sendTo(getPeer(), Packet::Channel::INTERACT);
}

void ServerPlayer::handleAssertion(Deserializer& d) {
	while (!d.atEnd()) {
		const auto field = d.read<NetField>();
		switch (field) {
		default:
			std::cout << Log::err << "Player received unhandled NetField, Type "
			          << static_cast<u32>(field) << "." << Log::endl;
			break;
		
		case NetField::POS:
			setPos(d.read<vec3>());
			break;
			
		case NetField::VEL:
			setVel(d.read<vec3>());
			break;
			
		case NetField::LOOK_YAW:
			setPitch(d.read<f32>());
			break;
			
		case NetField::LOOK_PITCH:
			setYaw(d.read<f32>());
			break;
			
		case NetField::FLYING:
			setFlying(d.read<bool>());
			break;
		
		case NetField::HAND_INV:
			setHandList(d.read<string>());
			break;
			
		case NetField::WIELD_INV:
			setWieldList(d.read<string>());
			break;
			
		case NetField::WIELD_INDEX:
			setWieldIndex(d.read<u16>());
			break;
		}
	}
}

void ServerPlayer::setDim(DimensionPtr dim, bool assert) {
	Player::setDim(dim, assert);
	
	// Force a generation flush.
	lastPos = vec3(INFINITY);
	changedMapBlocks = true;
}

void ServerPlayer::setPos(vec3 pos, bool assert) {
	vec3 lastMapBlock = Space::MapBlock::world::fromBlock(this->pos);
	vec3 newMapBlock = Space::MapBlock::world::fromBlock(pos);
	
	Player::setPos(pos, assert);
	
	if (newMapBlock != lastMapBlock && !changedMapBlocks) changedMapBlocks = true;
}

InventoryPtr ServerPlayer::getInventory() {
	return inventory;
}

ENetPeer* ServerPlayer::getPeer() {
	return client.peer;
}