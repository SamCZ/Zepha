//
// Created by aurailus on 11/01/19.
//

#include "ServerPlayer.h"

ServerPlayer::ServerPlayer(ServerPeer *peer) {
    this->peer = peer;
}

ServerPlayer::~ServerPlayer() {

}

Packet ServerPlayer::getInitPacket() {
    Packet p(PacketType::PLAYERINFO);

    Serializer::encodeFloat(p.data, pos.x);
    Serializer::encodeFloat(p.data, pos.y);
    Serializer::encodeFloat(p.data, pos.z);

    return p;
}
