//
// Created by aurailus on 04/04/19.
//

#ifndef ZEUS_REGION_H
#define ZEUS_REGION_H


#include <vec3.hpp>
#include <array>
#include "MapBlock.h"
#include "../../util/TransPos.h"

class Region {
public:
    explicit Region(glm::vec3 pos);

    MapBlock* operator[](int index);
    void set(int index, MapBlock* block);

    ~Region();
private:
    glm::vec3 pos {};

    const static int arrayLength = TransPos::REGION_SIZE * TransPos::REGION_SIZE * TransPos::REGION_SIZE;
    std::array<MapBlock*, arrayLength> mapBlocks {};
};

#endif //ZEUS_REGION_H
