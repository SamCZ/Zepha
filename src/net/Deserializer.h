//
// Created by aurailus on 2020-01-06.
//

#pragma once

#include <string>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

class Deserializer {
public:
    Deserializer(const std::string& data) : data(&data[0]), len(data.length()) {};
    Deserializer(const char* start, size_t len) : data(start), len(len) {};

    template <typename T> inline T read() { assert(false); };
    template <typename T> inline Deserializer& read(T& ref) {
        ref = read<T>();
        return *this;
    };

    bool atEnd() {
        return ind >= len;
    };

    const char* data;
    size_t len;
    size_t ind = 0;

private:
    typedef union { int in; char bytes[4]; }            int_union;
    typedef union { unsigned int in; char bytes[4]; }   uint_union;
    typedef union { short sh; char bytes[2]; }          short_union;
    typedef union { unsigned short sh; char bytes[2]; } ushort_union;
    typedef union { float fl; char bytes[4]; }          float_union;
};

template <> inline int Deserializer::read<int>() {
    int_union cv;
    cv.bytes[0] = data[ind];
    cv.bytes[1] = data[ind+1];
    cv.bytes[2] = data[ind+2];
    cv.bytes[3] = data[ind+3];
    ind += 4;
    return cv.in;
}

template <> inline unsigned int Deserializer::read<unsigned int>() {
    uint_union cv;
    cv.bytes[0] = data[ind];
    cv.bytes[1] = data[ind+1];
    cv.bytes[2] = data[ind+2];
    cv.bytes[3] = data[ind+3];
    ind += 4;
    return cv.in;
}

template <> inline short Deserializer::read<short>() {
    short_union cv;
    cv.bytes[0] = data[ind];
    cv.bytes[1] = data[ind+1];
    ind += 2;
    return cv.sh;
}

template <> inline unsigned short Deserializer::read<unsigned short>() {
    ushort_union cv;
    cv.bytes[0] = data[ind];
    cv.bytes[1] = data[ind+1];
    ind += 2;
    return cv.sh;
}

template <> inline char Deserializer::read<char>() {
    return data[ind++];
}

template <> inline unsigned char Deserializer::read<unsigned char>() {
    return static_cast<unsigned char>(data[ind++]);
}

template <> inline bool Deserializer::read<bool>() {
    return read<char>();
}

template <> inline float Deserializer::read<float>() {
    float_union cv;
    cv.bytes[0] = data[ind];
    cv.bytes[1] = data[ind+1];
    cv.bytes[2] = data[ind+2];
    cv.bytes[3] = data[ind+3];
    ind += 4;
    return cv.fl;
}

template <> inline std::string Deserializer::read<std::string>() {
    unsigned int len = read<unsigned int>();
    size_t i = ind;
    ind += len;
    return std::string(&data[i], len);
}

template <> inline glm::vec2 Deserializer::read<glm::vec2>() {
    return {
        read<float>(),
        read<float>()
    };
}

template <> inline glm::ivec2 Deserializer::read<glm::ivec2>() {
    return {
        read<int>(),
        read<int>()
    };
}

template <> inline glm::vec3 Deserializer::read<glm::vec3>() {
    return {
        read<float>(),
        read<float>(),
        read<float>()
    };
}

template <> inline glm::ivec3 Deserializer::read<glm::ivec3>() {
    return {
        read<int>(),
        read<int>(),
        read<int>()
    };
}

template <> inline std::vector<int> Deserializer::read<std::vector<int>>() {
    unsigned int len = read<unsigned int>();
    auto oldInd = ind;
    ind += len * 4;
    return std::vector<int>(
            reinterpret_cast<const int*>(&data[oldInd]),
            reinterpret_cast<const int*>(&data[ind]));
}

template <> inline std::vector<unsigned int> Deserializer::read<std::vector<unsigned int>>() {
    unsigned int len = read<unsigned int>();
    auto oldInd = ind;
    ind += len * 4;
    return std::vector<unsigned int>(
            reinterpret_cast<const unsigned int*>(&data[oldInd]),
            reinterpret_cast<const unsigned int*>(&data[ind]));
}

template <> inline std::vector<short> Deserializer::read<std::vector<short>>() {
    unsigned int len = read<unsigned int>();
    auto oldInd = ind;
    ind += len * 2;
    return std::vector<short>(
            reinterpret_cast<const short*>(&data[oldInd]),
            reinterpret_cast<const short*>(&data[ind]));
}

template <> inline std::vector<unsigned short> Deserializer::read<std::vector<unsigned short>>() {
    unsigned int len = read<unsigned int>();
    auto oldInd = ind;
    ind += len * 2;
    return std::vector<unsigned short>(
            reinterpret_cast<const unsigned short*>(&data[oldInd]),
            reinterpret_cast<const unsigned short*>(&data[ind]));
}

template <> inline std::vector<char> Deserializer::read<std::vector<char>>() {
    unsigned int len = read<unsigned int>();
    auto oldInd = ind;
    ind += len;
    return std::vector<char>(
            reinterpret_cast<const char*>(&data[oldInd]),
            reinterpret_cast<const char*>(&data[ind]));
}

template <> inline std::vector<unsigned char> Deserializer::read<std::vector<unsigned char>>() {
    unsigned int len = read<unsigned int>();
    auto oldInd = ind;
    ind += len;
    return std::vector<unsigned char>(
            reinterpret_cast<const unsigned char*>(&data[oldInd]),
            reinterpret_cast<const unsigned char*>(&data[ind]));
}

template <> inline std::vector<float> Deserializer::read<std::vector<float>>() {
    unsigned int len = read<unsigned int>();
    auto oldInd = ind;
    ind += len * 4;
    return std::vector<float>(
            reinterpret_cast<const float*>(&data[oldInd]),
            reinterpret_cast<const float*>(&data[ind]));
}

template <> inline std::vector<std::string> Deserializer::read<std::vector<std::string>>() {
    unsigned int len = read<unsigned int>();
    std::vector<std::string> v {};
    v.reserve(len);
    for (unsigned int i = 0; i < len; i++) {
        v.push_back(read<std::string>());
    }
    return std::move(v);
}