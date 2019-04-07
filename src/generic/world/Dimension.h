//
// Created by aurailus on 04/04/19.
//

#ifndef ZEUS_DIMENSION_H
#define ZEUS_DIMENSION_H

#include <unordered_map>
#include <vec3.hpp>
#include "../helpers/VecUtils.h"
#include "Region.h"

class Dimension {
public:
    Dimension() = default;

    void addChunk(glm::vec3 pos, BlockChunk* chunk);
    BlockChunk* getChunk(glm::vec3 pos);

    int regionSize();

    Region* getRegion(glm::vec3 pos);

    ~Dimension();
private:
    std::unordered_map<glm::vec3, Region*, VecUtils::compareFunc> regions;
};


#endif //ZEUS_DIMENSION_H
