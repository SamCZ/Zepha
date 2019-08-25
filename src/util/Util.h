//
// Created by aurailus on 28/04/19.
//

#pragma once


#include <string>
#include <sstream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include "Log.h"

namespace Util {
    struct EnumClassHash {
        template <typename T>
        std::size_t operator()(T t) const
        {
            return static_cast<std::size_t>(t);
        }
    };

    static std::string floatToString(float val) {
        std::ostringstream out;
        out.precision(2);
        out << std::fixed << val;
        return out.str();
    }

    inline static std::string doubleToString(double val) {
        return floatToString((float)val);
    }

    static std::string vecToString(glm::vec3 vec) {
        std::ostringstream out;
        out << (int)vec.x << ", " << (int)vec.y << ", " << (int)vec.z;
        return out.str();
    }

    static std::string floatVecToString(glm::vec3 vec) {
        std::ostringstream out;
        out.precision(2);
        out << std::fixed << vec.x << ", " << std::fixed << vec.y << ", " << std::fixed << vec.z;
        return out.str();
    }

    inline static float packFloat(const glm::vec3& vec) {
        auto charX = static_cast<unsigned char>((vec.x + 1.0f) * 0.5f * 255.f);
        auto charY = static_cast<unsigned char>((vec.y + 1.0f) * 0.5f * 255.f);
        auto charZ = static_cast<unsigned char>((vec.z + 1.0f) * 0.5f * 255.f);

        unsigned int packedInt = (charX << 16) | (charY << 8) | charZ;
        return static_cast<float>(static_cast<double>(packedInt) / static_cast<double>(1 << 24));
    }

    inline static unsigned int intFromHexSegment(const std::string &t) {
        unsigned int x;
        std::stringstream ss;
        ss << std::hex << t;
        ss >> x;
        return x;
    }


    static glm::vec4 hexToColorVec(std::string hex) {
        glm::vec4 color {};

        if (hex[0] == '#') hex.erase(0, 1);
        else std::cout << Log::err << "Color string does not begin with hash!" << Log::endl;

        std::string r, g, b, a;

        if (hex.length() == 3 || hex.length() == 4) {
            r = hex.substr(0, 1);
            r += r;
            g = hex.substr(1, 1);
            g += g;
            b = hex.substr(2, 1);
            b += b;
            a = (hex.length() == 4) ? hex.substr(3, 1) : "f";
            a += a;
        }
        else if (hex.length() == 6 || hex.length() == 8) {
            r = hex.substr(0,2);
            g = hex.substr(2,2);
            b = hex.substr(4,2);
            a = (hex.length() == 8) ? hex.substr(6,2) : "ff";
        }
        else {
            std::cout << Log::err << "Color string is of incorrect length!" << Log::endl;
            return color;
        }

        color.r = intFromHexSegment(r) / 255.f;
        color.g = intFromHexSegment(g) / 255.f;
        color.b = intFromHexSegment(b) / 255.f;
        color.a = intFromHexSegment(a) / 255.f;

        return color;
    }
};

