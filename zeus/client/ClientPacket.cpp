//
// Created by aurailus on 10/01/19.
//

#include "ClientPacket.h"

ClientPacket::ClientPacket(int packetType, std::vector<unsigned char> data) {
    this->packetType = packetType;
}

ClientPacket::~ClientPacket() {
    delete data;
}