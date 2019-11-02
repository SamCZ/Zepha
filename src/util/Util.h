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

typedef unsigned short ushort;

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

    static std::string getKeyStr(ushort key) {
        switch (key) {
            default: return "";
            case 32: return " ";
            case 39: return "'";
            case 44: return ",";
            case 45: return "-";
            case 46: return ".";
            case 47: return "/";
            case 48: return "0";
            case 49: return "1";
            case 50: return "2";
            case 51: return "3";
            case 52: return "4";
            case 53: return "5";
            case 54: return "6";
            case 55: return "7";
            case 56: return "8";
            case 57: return "9";
            case 59: return ";";
            case 61: return "=";
            case 65: return "a";
            case 66: return "b";
            case 67: return "c";
            case 68: return "d";
            case 69: return "e";
            case 70: return "f";
            case 71: return "g";
            case 72: return "h";
            case 73: return "i";
            case 74: return "j";
            case 75: return "k";
            case 76: return "l";
            case 77: return "m";
            case 78: return "n";
            case 79: return "o";
            case 80: return "p";
            case 81: return "q";
            case 82: return "r";
            case 83: return "s";
            case 84: return "t";
            case 85: return "u";
            case 86: return "v";
            case 87: return "w";
            case 88: return "x";
            case 89: return "y";
            case 90: return "z";
            case 91: return "[";
            case 92: return "\\";
            case 93: return "]";
            case 96: return "`";
            case 161: return "world1";
            case 162: return "world2";
            case 256: return "escape";
            case 257: return "enter";
            case 258: return "tab";
            case 259: return "backspace";
            case 260: return "insert";
            case 261: return "delete";
            case 262: return "right";
            case 263: return "left";
            case 264: return "down";
            case 265: return "up";
            case 266: return "pageup";
            case 267: return "pagedown";
            case 268: return "home";
            case 269: return "end";
            case 280: return "capslock";
            case 281: return "scrolllock";
            case 282: return "numlock";
            case 283: return "printscreen";
            case 284: return "pause";
            case 290: return "f1";
            case 291: return "f2";
            case 292: return "f3";
            case 293: return "f4";
            case 294: return "f5";
            case 295: return "f6";
            case 296: return "f7";
            case 297: return "f8";
            case 298: return "f9";
            case 299: return "f10";
            case 300: return "f11";
            case 301: return "f12";
            case 302: return "f13";
            case 303: return "f14";
            case 304: return "f15";
            case 305: return "f16";
            case 306: return "f17";
            case 307: return "f18";
            case 308: return "f19";
            case 309: return "f20";
            case 310: return "f21";
            case 311: return "f22";
            case 312: return "f23";
            case 313: return "f24";
            case 314: return "f25";
            case 320: return "num0";
            case 321: return "num1";
            case 322: return "num2";
            case 323: return "num3";
            case 324: return "num4";
            case 325: return "num5";
            case 326: return "num6";
            case 327: return "num7";
            case 328: return "num8";
            case 329: return "num9";
            case 330: return "num.";
            case 331: return "num/";
            case 332: return "num*";
            case 333: return "num-";
            case 334: return "num+";
            case 335: return "numenter";
            case 336: return "num=";
            case 341: return "leftshift";
            case 342: return "leftctrl";
            case 343: return "leftalt";
            case 344: return "rightshift";
            case 345: return "rightctrl";
            case 346: return "rightalt";
            case 348: return "menu";
        }
    }
};

