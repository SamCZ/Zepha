//
// Created by aurailus on 28/04/19.
//

#ifndef ZEUS_UTIL_H
#define ZEUS_UTIL_H


#include <string>
#include <sstream>
#include <glm/vec3.hpp>

namespace Util {
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
};


#endif //ZEUS_UTIL_H
