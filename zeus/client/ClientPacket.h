//
// Created by aurailus on 10/01/19.
//

#ifndef ZEUS_CLIENTPACKET_H
#define ZEUS_CLIENTPACKET_H

#include "vector"

struct ClientPacket {
public:
    ClientPacket() = default;
    ClientPacket(int packetType, std::vector<unsigned char> data);

    int packetType;
    std::vector<unsigned char>* data;

    ~ClientPacket();
};


#endif //ZEUS_CLIENTPACKET_H
