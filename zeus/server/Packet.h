//
// Created by aurailus on 10/01/19.
//

#ifndef ZEUS_PACKET_H
#define ZEUS_PACKET_H

#include <vector>
#include <string>
#include <asio.hpp>

class Packet {
public:
    typedef unsigned long PacketType;
    typedef unsigned char PacketByte;

    Packet() = default;
    explicit Packet(PacketType p);

    void addIntegers(std::vector<int> &integers);
    void addInt(int integer);
    void addFloats(std::vector<float> &floats);
    void addFloat(float floatVal);
    void addString(std::string *string);
    void addString(std::string string);

    unsigned long length = 0;
    PacketType type = UNDEFINED;
    std::vector<PacketByte> data;

    ~Packet() = default;

    static Packet deserialize(std::vector<PacketByte> data);
    std::vector<Packet::PacketByte> serialize();

    static void  encodeInt(std::vector<PacketByte> &target, int num);
    static int   decodeInt(PacketByte* intStart);
    static void  encodeFloat(std::vector<PacketByte> &target, float num);
    static float decodeFloat(PacketByte* floatStart);
public:
    const static PacketType UNDEFINED = 0;
    const static PacketType HANDSHAKE = 1;
    const static PacketType AUTHTOKEN = 2;
    const static PacketType PLAYRINFO = 2;
//    const static PacketType REQCHUNKS = 2;
//    const static PacketType KEEPALIVE = 3;
};


#endif //ZEUS_PACKET_H
