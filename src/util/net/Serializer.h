//
// Created by aurailus on 06/02/19.
//

#pragma once

#include <string>
#include <vector>
#include <glm/vec3.hpp>

using namespace std;

class Serializer {
public:
    static void encodeInt       (string &target, int num);
    static void encodeUInt      (string &target, unsigned int num);
    static void encodeShort     (string &target, short num);
    static void encodeUShort    (string &target, unsigned short num);
    static void encodeFloat     (string &target, float num);
    static void encodeString    (string &target, string str);

    static void encodeFloatVec3 (string &target, glm::vec3 vec);
    static void encodeIntVec3   (string &target, glm::vec3 vec);

    static void encodeIntVec    (string &target, vector<int>& vec);
    static void encodeUIntVec   (string &target, vector<unsigned int>& vec);
    static void encodeUShortVec (string &target, vector<unsigned short>& vec);
    static void encodeFloatVec  (string &target, vector<float>& vec);

    static int            decodeInt       (const char* intStart);
    static unsigned int   decodeUInt      (const char* uIntStart);
    static short          decodeShort     (const char* shortStart);
    static unsigned short decodeUShort    (const char* uShortStart);
    static float          decodeFloat     (const char* floatStart);
    static string         decodeString    (const char* stringStart);

    static glm::vec3      decodeFloatVec3 (char* vecStart);
    static glm::vec3      decodeIntVec3   (char* vecStart);

    static vector<int>            decodeIntVec   (string &string);
    static vector<unsigned int>   decodeUIntVec  (string &string);
    static vector<unsigned short> decodeUShortVec(string &target);
    static vector<float>          decodeFloatVec (string &string);
private:
    typedef union { int in; char bytes[4]; }            int_union;
    typedef union { unsigned int in; char bytes[4]; }   uint_union;
    typedef union { short sh; char bytes[2]; }          short_union;
    typedef union { unsigned short sh; char bytes[2]; } ushort_union;
    typedef union { float fl; char bytes[4]; }          float_union;
};

