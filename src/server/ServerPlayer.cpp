//
// Created by aurailus on 11/01/19.
//

#include "ServerPlayer.h"

ServerPlayer::ServerPlayer(ServerPeer *peer) {
    this->peer = peer;
    peer->player = this;
}

ServerPlayer::~ServerPlayer() {

}

Packet ServerPlayer::getInitPacket() {
    Packet p(Packet::PLAYER_INFO);

    Serializer::encodeFloat(p.data, pos.x);
    Serializer::encodeFloat(p.data, pos.y);
    Serializer::encodeFloat(p.data, pos.z);

    return p;
}
