//
// Created by aurailus on 10/01/19.
//

#include "Packet.h"

Packet::Packet(Packet::PacketType p) {
    this->type = p;
}

//Convert a vector of PacketBytes (chars) into a PacketData by splitting the head and body information.
Packet* Packet::deserialize(std::vector<PacketByte> data) {
    if (data.size() < 4) std::cerr << "Packet does not contain data." << std::endl;

    //Seperate the packet header from the body,
    //This can be changed to support more header values in the future.

    //Determine Packet Type
    int num = 0;
    for (int i = 0; i < 4; i++) {
        num <<= 8;
        num |= data[i];
    }

    //Get body of the packet
    std::vector<PacketByte> dataBody;
    dataBody.reserve(data.size() - 4);

    for (int i = 4; i < data.size(); i++) {
        dataBody.push_back(data[i]);
    }

    auto p = new Packet();
    p->type = (PacketType)num;
    p->length = dataBody.size();
    p->data = std::move(dataBody);

    return p;
}

//Convert a PacketData into a serialized form to send over UDP.
std::vector<Packet::PacketByte> Packet::serialize() {
    std::vector<PacketByte> data;
    data.reserve(this->length + 4);

    //Encode Packet Type
    encodeInt(data, (int)this->type);

    //Add body of the packet
    for (PacketByte i : this->data) {
        data.push_back(i);
    }

    return data;
}

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wconversion"
void Packet::encodeInt(std::vector<Packet::PacketByte> &target, int num) {
    target.push_back((num >> 24) & 0xFF);
    target.push_back((num >> 16) & 0xFF);
    target.push_back((num >> 8) & 0xFF);
    target.push_back((num) & 0xFF);
}
#pragma clang diagnostic pop

void Packet::encodeFloat(std::vector<Packet::PacketByte> &target, float num) {
    typedef union {
        float a;
        unsigned char b[4];
    } float_union;

    float_union fl {num};

    target.push_back(fl.b[0]);
    target.push_back(fl.b[1]);
    target.push_back(fl.b[2]);
    target.push_back(fl.b[3]);
}

float Packet::decodeFloat(Packet::PacketByte *floatStart) {
    typedef union {
        float a;
        unsigned char b[4];
    } float_union;

    float_union fl { .b = {*floatStart, *(floatStart+1), *(floatStart+2), *(floatStart+3)} };

    return fl.a;
}

int Packet::decodeInt(PacketByte* intStart) {
    int num = 0;
    for (int i = 0; i < 4; i++) {
        num <<= 8;
        num |= *(intStart++);
    }
    return num;
}

void Packet::addIntegers(std::vector<int> &integers) {
    for (int i : integers) {
        encodeInt(this->data, i);
    }
    this->length = data.size();
}

void Packet::addInt(int integer) {
    encodeInt(this->data, integer);
    this->length = data.size();
}

void Packet::addFloats(std::vector<float> &floats) {
    for (float f : floats) {
        encodeFloat(this->data, f);
    }
    this->length = data.size();
}

void Packet::addFloat(float floatVal) {
    encodeFloat(this->data, floatVal);
    this->length = data.size();
}

void Packet::addString(std::string *string) {
    addInt((int)string->length());
    std::copy(string->begin(), string->end(), std::back_inserter(data));
    this->length = data.size();
}

void Packet::addString(std::string string) {
    addString(&string);
}