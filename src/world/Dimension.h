//
// Created by aurailus on 04/04/19.
//

#ifndef ZEUS_DIMENSION_H
#define ZEUS_DIMENSION_H


#include <vec3.hpp>
#include <unordered_map>
#include "region/Region.h"
#include "../util/Vec.h"

class Dimension {
public:
    Dimension() = default;

    void addChunk(glm::vec3 pos, std::shared_ptr<BlockChunk> chunk);
    std::shared_ptr<BlockChunk> getChunk(glm::vec3 pos);

    Region* getRegion(glm::vec3 pos);

    ~Dimension();
private:
    std::unordered_map<glm::vec3, Region*, VecUtils::compareFunc> regions;
};


#endif //ZEUS_DIMENSION_H
