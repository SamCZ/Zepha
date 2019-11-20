//
// Created by aurailus on 06/02/19.
//

#include "Serializer.h"

void Serializer::encodeInt(string &target, int num) {
    int_union cv = {num};

    target += cv.bytes[0];
    target += cv.bytes[1];
    target += cv.bytes[2];
    target += cv.bytes[3];
}

void Serializer::encodeUInt(string &target, unsigned int num) {
    uint_union cv = {num};

    target += cv.bytes[0];
    target += cv.bytes[1];
    target += cv.bytes[2];
    target += cv.bytes[3];
}

void Serializer::encodeShort(string &target, short num) {
    short_union cv = {num};

    target += cv.bytes[0];
    target += cv.bytes[1];
}

void Serializer::encodeUShort(string &target, unsigned short num) {
    ushort_union cv = {num};

    target += cv.bytes[0];
    target += cv.bytes[1];
}

void Serializer::encodeFloat(string &target, float num) {
    float_union cv {num};

    target += cv.bytes[0];
    target += cv.bytes[1];
    target += cv.bytes[2];
    target += cv.bytes[3];
}

void Serializer::encodeString(string &target, string str) {
    encodeInt(target, (int)str.length());
    target += str;
}

void Serializer::encodeFloatVec3(string &target, glm::vec3 vec) {
    encodeFloat(target, vec.x);
    encodeFloat(target, vec.y);
    encodeFloat(target, vec.z);
}

void Serializer::encodeIntVec3(string &target, glm::vec3 vec) {
    encodeInt(target, static_cast<int>(vec.x));
    encodeInt(target, static_cast<int>(vec.y));
    encodeInt(target, static_cast<int>(vec.z));
}

void Serializer::encodeIntVec(string &target, vector<int> &vec) {
    //Used a reinterpret cast to speed things up
    target.reserve(target.length() + vec.size() * 4);
    target += string(reinterpret_cast<const char*>(&vec[0]), vec.size()*4);
}

void Serializer::encodeUIntVec(string &target, vector<unsigned int> &vec) {
    //Used a reinterpret cast to speed things up
    target.reserve(target.length() + vec.size() * 4);
    target += string(reinterpret_cast<const char*>(&vec[0]), vec.size()*4);
}

void Serializer::encodeUShortVec(string &target, vector<unsigned short> &vec) {
    //Used a reinterpret cast to speed things up
    target.reserve(target.length() + vec.size() * 2);
    target += string(reinterpret_cast<const char*>(&vec[0]), vec.size()*2);
}

void Serializer::encodeFloatVec(string &target, vector<float> &vec){
    //Used a reinterpret cast to speed things up
    target.reserve(target.length() + vec.size() * 4);
    target += string(reinterpret_cast<const char*>(&vec[0]), vec.size()*4);
}

int Serializer::decodeInt(char *in) {
    int_union cv;
    cv.bytes[0] = *(in);
    cv.bytes[1] = *(in+1);
    cv.bytes[2] = *(in+2);
    cv.bytes[3] = *(in+3);

    return cv.in;
}

unsigned int Serializer::decodeUInt(char *in) {
    uint_union cv;
    cv.bytes[0] = *(in);
    cv.bytes[1] = *(in+1);
    cv.bytes[2] = *(in+2);
    cv.bytes[3] = *(in+3);

    return cv.in;
}

short Serializer::decodeShort(char *in) {
    short_union cv;
    cv.bytes[0] = *(in);
    cv.bytes[1] = *(in+1);

    return cv.sh;
}

unsigned short Serializer::decodeUShort(char *in) {
    short_union cv;
    cv.bytes[0] = *(in);
    cv.bytes[1] = *(in+1);

    return cv.sh;
}

float Serializer::decodeFloat(char *in) {
    float_union cv;
    cv.bytes[0] = *(in);
    cv.bytes[1] = *(in+1);
    cv.bytes[2] = *(in+2);
    cv.bytes[3] = *(in+3);

    return cv.fl;
}

string Serializer::decodeString(char *stringStart) {
    int len = decodeInt(stringStart);

    return string(stringStart + 4, stringStart + 4 + len);
}

vector<int> Serializer::decodeIntVec(string &string) {
    //Reverse the conversion done to vectors in the encode*Vec classes using reinterpret_cast.
    return std::vector<int>(reinterpret_cast<const int*>(&string[0]),
                            reinterpret_cast<const int*>(&string[string.size()]));
}

vector<unsigned int> Serializer::decodeUIntVec(string &string) {
    //Reverse the conversion done to vectors in the encode*Vec classes using reinterpret_cast.
    return std::vector<unsigned int>(reinterpret_cast<const int*>(&string[0]),
                                     reinterpret_cast<const int*>(&string[string.size()]));
}

vector<unsigned short> Serializer::decodeUShortVec(string &string) {
    //Reverse the conversion done to vectors in the encode*Vec classes using reinterpret_cast.
    return std::vector<unsigned short>(reinterpret_cast<const unsigned short*>(&string[0]),
                                     reinterpret_cast<const unsigned short*>(&string[string.size()]));
}


vector<float> Serializer::decodeFloatVec(string &string) {
    //Reversed the conversion done to vectors in the encode*Vec classes using reinterpret_cast.
    return std::vector<float>(reinterpret_cast<const float*>(&string[0]),
                              reinterpret_cast<const float*>(&string[string.size()]));
}

glm::vec3 Serializer::decodeFloatVec3(char *vecStart) {
    return {
        decodeFloat(vecStart),
        decodeFloat(vecStart + 4),
        decodeFloat(vecStart + 8)
    };
}

glm::vec3 Serializer::decodeIntVec3(char *vecStart) {
    return {
        decodeInt(vecStart),
        decodeInt(vecStart + 4),
        decodeInt(vecStart + 8)
    };
}