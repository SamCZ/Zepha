//
// Created by aurailus on 28/04/19.
//

#ifndef ZEUS_UTIL_H
#define ZEUS_UTIL_H


#include <string>
#include <sstream>
#include <vec3.hpp>

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
};


#endif //ZEUS_UTIL_H
