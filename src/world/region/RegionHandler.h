//
// Created by aurailus on 07/04/19.
//

#ifndef ZEUS_REGIONHANDLER_H
#define ZEUS_REGIONHANDLER_H

#include <vec3.hpp>
#include <unordered_map>
#include "Region.h"
#include "../../util/Vec.h"

template <class T>
class RegionHandler {
public:
    RegionHandler() = default;

    void addChunk(glm::vec3 pos, std::shared_ptr<T> chunk);
    std::shared_ptr<T> getChunk(glm::vec3 pos);

    Region<T>* getRegion(glm::vec3 pos);

    ~RegionHandler();
private:
    std::unordered_map<glm::vec3, Region<T>*, VecUtils::compareFunc> regions;
};

#include "RegionHandler.inl"

#endif //ZEUS_REGIONHANDLER_H
