//
// Created by aurailus on 29/06/19.
//

#include <iostream>

#include "ServerClient.h"

#include "../../Packet.h"
#include "../../Serializer.h"
#include "../../Deserializer.h"
#include "../../client/NetPlayerField.h"
#include "../../../game/inventory/InventoryRefs.h"

ServerClient::ServerClient(ENetPeer *peer, ENetAddress address, InventoryRefs& refs) :
    peer(peer),
    address(address),
    cid(peer->connectID),
    inventory(refs.createInv("player:" + std::to_string(cid))) {
}

void ServerClient::setUsername(const std::string &name) {
    this->username = name;
}

glm::vec3 ServerClient::getPos() {
    return pos;
}

void ServerClient::setPos(glm::vec3 pos, bool assert) {
    glm::vec3 lastMapBlock = Space::MapBlock::world::fromBlock(this->pos);
    glm::vec3 newMapBlock = Space::MapBlock::world::fromBlock(pos);

    if (newMapBlock != lastMapBlock && !changedMapBlocks) changedMapBlocks = true;
    this->pos = pos;

    if (assert) assertField(NetPlayerField::POSITION, pos);
}

glm::vec3 ServerClient::getVel() {
    return vel;
}

void ServerClient::setVel(glm::vec3 vel, bool assert) {
    this->vel = vel;
    if (assert) assertField(NetPlayerField::VELOCITY, vel);
}

float ServerClient::getPitch() {
    return pitch;
}

void ServerClient::setPitch(float pitch, bool assert) {
    this->pitch = pitch;
    if (assert) assertField(NetPlayerField::PITCH, pitch);
}

float ServerClient::getYaw() {
    return yaw;
}

void ServerClient::setYaw(float yaw, bool assert) {
    this->yaw = yaw;
    if (assert) assertField(NetPlayerField::YAW, yaw);
}

bool ServerClient::isFlying() {
    return flying;
}

void ServerClient::setFlying(bool flying, bool assert) {
    this->flying = flying;
    if (assert) assertField(NetPlayerField::FLYING, flying);
}

std::shared_ptr<Inventory> ServerClient::getInventory() {
    return inventory;
}

std::shared_ptr<InventoryList> ServerClient::getHandList() {
    return (*inventory)[handList];
}

void ServerClient::setHandList(const std::string& list, bool assert) {
    if (list.empty() || !(*inventory)[list]) this->handList = "";
    else this->handList = list;

    if (assert) assertField(NetPlayerField::HAND_INV, this->handList);
}

std::shared_ptr<InventoryList> ServerClient::getWieldList() {
    return (*inventory)[wieldList];
}

void ServerClient::setWieldList(const std::string& list, bool assert) {
    if (list.empty() || !(*inventory)[list]) this->wieldList = "";
    else this->wieldList = list;

    if (assert) assertField(NetPlayerField::WIELD_INV, this->wieldList);
    setWieldIndex(wieldIndex, assert);
}

unsigned short ServerClient::getWieldIndex() {
    return wieldIndex;
}

void ServerClient::setWieldIndex(unsigned short index, bool assert) {
    auto wieldList = (*inventory)[this->wieldList];
    wieldIndex = index % (wieldList ? wieldList->getLength() : 1);
    if (assert) assertField(NetPlayerField::WIELD_INDEX, static_cast<unsigned short>(wieldIndex));
}

void ServerClient::handleAssertion(Deserializer &d) {
    while (!d.atEnd()) {
        switch (static_cast<NetPlayerField>(d.read<unsigned int>())) {
            case NetPlayerField::POSITION: setPos(d.read<glm::vec3>()); break;
            case NetPlayerField::VELOCITY: setVel(d.read<glm::vec3>()); break;
            case NetPlayerField::PITCH: setPitch(d.read<float>()); break;
            case NetPlayerField::YAW: setYaw(d.read<float>()); break;

            case NetPlayerField::FLYING: setFlying(d.read<bool>()); break;

            case NetPlayerField::HAND_INV: setHandList(d.read<std::string>()); break;
            case NetPlayerField::WIELD_INV: setWieldList(d.read<std::string>()); break;
            case NetPlayerField::WIELD_INDEX: setWieldIndex(d.read<unsigned short>()); break;
        }
    }
}

//void ServerClient::setMapBlockIntegrity(glm::ivec3 pos, unsigned long long integrity) {
//    mapBlockIntegrity[pos] = integrity;
//}
//
//unsigned long long ServerClient::getMapBlockIntegrity(glm::ivec3 pos) {
//    if (mapBlockIntegrity.count(pos)) return mapBlockIntegrity[pos];
//    return 0;
//}

template <typename T>
void ServerClient::assertField(NetPlayerField field, T data) {
    Serializer().append(static_cast<unsigned int>(field)).append<T>(data)
        .packet(PacketType::THIS_PLAYER_INFO).sendTo(peer, PacketChannel::INTERACT);
}