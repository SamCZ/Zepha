//
// Created by aurailus on 19/03/19.
//

#ifndef ZEUS_VEC3COMPAREFUNC_H
#define ZEUS_VEC3COMPAREFUNC_H


#include <cstddef>
#include <functional>
#include <vec3.hpp>

namespace Vec3Compare {
    struct func {
        size_t operator()(const glm::vec3& k)const {
            return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
        }
    };
};


#endif //ZEUS_VEC3COMPAREFUNC_H
