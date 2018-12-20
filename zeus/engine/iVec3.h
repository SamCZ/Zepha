//
// Created by aurailus on 19/12/18.
//

#ifndef ZEUS_IVEC3_H
#define ZEUS_IVEC3_H

struct iVec3 {
    int x, y, z;

    iVec3() {
        x = 0;
        y = 0;
        z = 0;
    }

    iVec3(int x, int y, int z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

//    iVec3(int n) {
//        this->x = n;
//        this->y = n;
//        this->z = n;
//    }
};

#endif //ZEUS_IVEC3_H

namespace std {
    template<> struct hash<iVec3> {
        std::size_t operator()(const iVec3 &k) const {
            return (k.x * 97 + k.y) * 89 + k.z;
        }
    };

    template<> struct less<iVec3> {
        bool operator()(const iVec3 &a, const iVec3 &b) const {
            if (a.x < b.x) return true;
            if (a.y < b.y) return true;
            if (a.z < b.z) return true;
            return false;
        }
    };

    template<> struct equal_to<iVec3> {
        bool operator()(const iVec3 &a, const iVec3 &b) const {
            return (a.x == b.x && a.y == b.y && a.z == b.z);
        }
    };
}