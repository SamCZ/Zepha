//
// Created by aurailus on 10/01/19.
//

#ifndef ZEUS_PACKET_H
#define ZEUS_PACKET_H

#include <vector>

class Packet {
public:
    typedef unsigned long PacketType;
    typedef unsigned char PacketByte;

    Packet() = default;
    explicit Packet(PacketType p);

    void addIntegers(std::vector<int> &integers);
    void addInt(int integer);

    unsigned long length;
    PacketType type;
    std::vector<PacketByte> data;

    ~Packet() = default;

    static Packet deserialize(std::vector<PacketByte> data);
    std::vector<Packet::PacketByte> serialize();

    static void encodeInt(std::vector<PacketByte> &target, int num);
    static int  decodeInt(PacketByte* intStart);
public:
    const static PacketType HANDSHAKE = 0;
    const static PacketType REQCHUNKS = 1;
    const static PacketType KEEPALIVE = 2;
};


#endif //ZEUS_PACKET_H
