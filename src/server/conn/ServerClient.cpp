//
// Created by aurailus on 29/06/19.
//

#include <iostream>

#include "ServerClient.h"

#include "../../util/net/Packet.h"
#include "../../util/net/Serializer.h"
#include "../../game/inventory/InventoryRefs.h"
#include "../../game/scene/net/NetPlayerField.h"

ServerClient::ServerClient(ENetPeer *peer, ENetAddress address, DefinitionAtlas& defs, InventoryRefs& refs) :
    peer(peer),
    address(address),
    cid(peer->connectID),
    inventory(refs.createInv("player:" + std::to_string(cid))) {
}

void ServerClient::setUsername(const std::string &name) {
    this->username = name;
}

void ServerClient::assertPos(glm::vec3 pos) {
    setPos(pos);
    Serializer()
        .append(static_cast<unsigned int>(NetPlayerField::POSITION))
        .append(pos)
        .packet(PacketType::THIS_PLAYER_INFO)
        .sendTo(peer, PacketChannel::PLAYER);
}


void ServerClient::setPos(glm::vec3 pos) {
    glm::vec3 lastMapBlock = Space::MapBlock::world::fromBlock(this->pos);
    glm::vec3 newMapBlock = Space::MapBlock::world::fromBlock(pos);

    if (newMapBlock != lastMapBlock && !changedMapBlocks) changedMapBlocks = true;
    this->pos = pos;
}

glm::vec3 ServerClient::getPos() {
    return pos;
}

void ServerClient::assertPitch(float pitch) {
    setPitch(pitch);
    Serializer()
        .append(static_cast<unsigned int>(NetPlayerField::PITCH))
        .append(pitch)
        .packet(PacketType::THIS_PLAYER_INFO)
        .sendTo(peer, PacketChannel::PLAYER);
}

void ServerClient::setPitch(float pitch) {
    this->pitch = pitch;
}

float ServerClient::getPitch() {
    return pitch;
}

void ServerClient::assertYaw(float yaw) {
    setYaw(yaw);
    Serializer()
            .append(static_cast<unsigned int>(NetPlayerField::YAW))
            .append(yaw)
            .packet(PacketType::THIS_PLAYER_INFO)
            .sendTo(peer, PacketChannel::PLAYER);
}

void ServerClient::setYaw(float yaw) {
    this->yaw = yaw;
}

float ServerClient::getYaw() {
    return yaw;
}

void ServerClient::setFlying(bool flying) {
    this->flying = flying;
}

bool ServerClient::isFlying() {
    return flying;
}

void ServerClient::setMapBlockIntegrity(glm::ivec3 pos, unsigned long long integrity) {
    mapBlockIntegrity[pos] = integrity;
}

unsigned long long ServerClient::getMapBlockIntegrity(glm::ivec3 pos) {
    if (mapBlockIntegrity.count(pos)) return mapBlockIntegrity[pos];
    return 0;
}

std::shared_ptr<Inventory> ServerClient::getInventory() {
    return inventory;
}