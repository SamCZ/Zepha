//
// Created by aurailus on 29/06/19.
//

#include "ServerPlayer.h"

#include "world/ServerWorld.h"
#include "util/net/NetField.h"
#include "server/ServerClient.h"
#include "util/net/Serializer.h"
#include "util/net/Deserializer.h"
#include "world/inv/ServerInventoryRefs.h"

ServerPlayer::ServerPlayer(ServerClient& client, World& world, SubgamePtr game, DimensionPtr dim) :
    Player(game, world, dim, client.id), Entity(game, dim), client(client),
    inventory(world.getRefs()->createInventory("player:" + std::to_string(id))) {}

void ServerPlayer::assertField(Packet packet) {
    packet.type = Packet::Type::THIS_PLAYER_INFO;
    packet.sendTo(getPeer(), Packet::Channel::INTERACT);
}

void ServerPlayer::handleAssertion(Deserializer &d) {
    while (!d.atEnd()) {
        switch (d.readE<NetField>()) {
            case NetField::POS:         setPos(d.read<glm::vec3>()); break;
            case NetField::VEL:         setVel(d.read<glm::vec3>()); break;
            case NetField::LOOK_YAW:    setPitch(d.read<float>());   break;
            case NetField::LOOK_PITCH:  setYaw(d.read<float>());     break;
            case NetField::FLYING:      setFlying(d.read<bool>());   break;

            case NetField::HAND_INV:    setHandList(d.read<std::string>()); break;
            case NetField::WIELD_INV:   setWieldList(d.read<std::string>());   break;
            case NetField::WIELD_INDEX: setWieldIndex(d.read<unsigned short>()); break;
        }
    }
}

void ServerPlayer::setPos(glm::vec3 pos, bool assert) {
    glm::vec3 lastMapBlock = Space::MapBlock::world::fromBlock(this->pos);
    glm::vec3 newMapBlock = Space::MapBlock::world::fromBlock(pos);

    Player::setPos(pos, assert);

    if (newMapBlock != lastMapBlock && !changedMapBlocks) changedMapBlocks = true;
}

InventoryPtr ServerPlayer::getInventory() {
    return inventory;
}

ENetPeer* ServerPlayer::getPeer() {
    return client.peer;
}
