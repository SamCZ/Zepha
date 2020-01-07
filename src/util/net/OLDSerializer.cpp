//
// Created by aurailus on 06/02/19.
//

#include "OLDSerializer.h"

void OLDSerializer::encodeInt(string &target, int num) {
    int_union cv = {num};

    target += cv.bytes[0];
    target += cv.bytes[1];
    target += cv.bytes[2];
    target += cv.bytes[3];
}

void OLDSerializer::encodeUInt(string &target, unsigned int num) {
    uint_union cv = {num};

    target += cv.bytes[0];
    target += cv.bytes[1];
    target += cv.bytes[2];
    target += cv.bytes[3];
}

void OLDSerializer::encodeShort(string &target, short num) {
    short_union cv = {num};

    target += cv.bytes[0];
    target += cv.bytes[1];
}

void OLDSerializer::encodeUShort(string &target, unsigned short num) {
    ushort_union cv = {num};

    target += cv.bytes[0];
    target += cv.bytes[1];
}

void OLDSerializer::encodeFloat(string &target, float num) {
    float_union cv {num};

    target += cv.bytes[0];
    target += cv.bytes[1];
    target += cv.bytes[2];
    target += cv.bytes[3];
}

void OLDSerializer::encodeString(string &target, string str) {
    encodeInt(target, (int)str.length());
    target += str;
}

void OLDSerializer::encodeFloatVec3(string &target, glm::vec3 vec) {
    encodeFloat(target, vec.x);
    encodeFloat(target, vec.y);
    encodeFloat(target, vec.z);
}

void OLDSerializer::encodeIntVec3(string &target, glm::vec3 vec) {
    encodeInt(target, static_cast<int>(vec.x));
    encodeInt(target, static_cast<int>(vec.y));
    encodeInt(target, static_cast<int>(vec.z));
}

void OLDSerializer::encodeIntVec(string &target, vector<int> &vec) {
    //Used a reinterpret cast to speed things up
    target.reserve(target.length() + vec.size() * 4);
    target += string(reinterpret_cast<const char*>(&vec[0]), vec.size()*4);
}

void OLDSerializer::encodeUIntVec(string &target, vector<unsigned int> &vec) {
    //Used a reinterpret cast to speed things up
    target.reserve(target.length() + vec.size() * 4);
    target += string(reinterpret_cast<const char*>(&vec[0]), vec.size()*4);
}

void OLDSerializer::encodeUShortVec(string &target, vector<unsigned short> &vec) {
    //Used a reinterpret cast to speed things up
    target.reserve(target.length() + vec.size() * 2);
    target += string(reinterpret_cast<const char*>(&vec[0]), vec.size()*2);
}

void OLDSerializer::encodeFloatVec(string &target, vector<float> &vec){
    //Used a reinterpret cast to speed things up
    target.reserve(target.length() + vec.size() * 4);
    target += string(reinterpret_cast<const char*>(&vec[0]), vec.size()*4);
}

int OLDSerializer::decodeInt(const char *in) {
    int_union cv;
    cv.bytes[0] = *(in);
    cv.bytes[1] = *(in+1);
    cv.bytes[2] = *(in+2);
    cv.bytes[3] = *(in+3);

    return cv.in;
}

unsigned int OLDSerializer::decodeUInt(const char *in) {
    uint_union cv;
    cv.bytes[0] = *(in);
    cv.bytes[1] = *(in+1);
    cv.bytes[2] = *(in+2);
    cv.bytes[3] = *(in+3);

    return cv.in;
}

short OLDSerializer::decodeShort(const char *in) {
    short_union cv;
    cv.bytes[0] = *(in);
    cv.bytes[1] = *(in+1);

    return cv.sh;
}

unsigned short OLDSerializer::decodeUShort(const char *in) {
    short_union cv;
    cv.bytes[0] = *(in);
    cv.bytes[1] = *(in+1);

    return cv.sh;
}

float OLDSerializer::decodeFloat(const char *in) {
    float_union cv;
    cv.bytes[0] = *(in);
    cv.bytes[1] = *(in+1);
    cv.bytes[2] = *(in+2);
    cv.bytes[3] = *(in+3);

    return cv.fl;
}

string OLDSerializer::decodeString(const char *stringStart) {
    int len = decodeInt(stringStart);

    return string(stringStart + 4, stringStart + 4 + len);
}

vector<int> OLDSerializer::decodeIntVec(const string &string) {
    //Reverse the conversion done to vectors in the encode*Vec classes using reinterpret_cast.
    return std::vector<int>(reinterpret_cast<const int*>(&string[0]),
                            reinterpret_cast<const int*>(&string[string.size()]));
}

vector<unsigned int> OLDSerializer::decodeUIntVec(const string &string) {
    //Reverse the conversion done to vectors in the encode*Vec classes using reinterpret_cast.
    return std::vector<unsigned int>(reinterpret_cast<const int*>(&string[0]),
                                     reinterpret_cast<const int*>(&string[string.size()]));
}

vector<unsigned short> OLDSerializer::decodeUShortVec(const string &string) {
    //Reverse the conversion done to vectors in the encode*Vec classes using reinterpret_cast.
    return std::vector<unsigned short>(reinterpret_cast<const unsigned short*>(&string[0]),
                                     reinterpret_cast<const unsigned short*>(&string[string.size()]));
}


vector<float> OLDSerializer::decodeFloatVec(const string &string) {
    //Reversed the conversion done to vectors in the encode*Vec classes using reinterpret_cast.
    return std::vector<float>(reinterpret_cast<const float*>(&string[0]),
                              reinterpret_cast<const float*>(&string[string.size()]));
}

glm::vec3 OLDSerializer::decodeFloatVec3(const char *vecStart) {
    return {
        decodeFloat(vecStart),
        decodeFloat(vecStart + 4),
        decodeFloat(vecStart + 8)
    };
}

glm::vec3 OLDSerializer::decodeIntVec3(const char *vecStart) {
    return {
        decodeInt(vecStart),
        decodeInt(vecStart + 4),
        decodeInt(vecStart + 8)
    };
}