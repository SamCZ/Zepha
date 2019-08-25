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
    static void encodeFloat     (string &target, float num);
    static void encodeString    (string &target, string str);
    static void encodeFloatVec3 (string &target, glm::vec3 vec);
    static void encodeIntVec3   (string &target, glm::vec3 vec);

    static void encodeIntVec  (string &target, vector<int>& vec);
    static void encodeUIntVec (string &target, vector<unsigned int>& vec);
    static void encodeFloatVec(string &target, vector<float>& vec);

    static int       decodeInt       (char* intStart);
    static float     decodeFloat     (char* floatStart);
    static string    decodeString    (char* stringStart);
    static glm::vec3 decodeFloatVec3 (char* vecStart);
    static glm::vec3 decodeIntVec3   (char* vecStart);

    static vector<int>          decodeIntVec  (string &string);
    static vector<unsigned int> decodeUIntVec (string &string);
    static vector<float>        decodeFloatVec(string &string);
private:
    typedef union {
        float fl;
        int   in;
        char bytes[4];
    } convert_union;
};

