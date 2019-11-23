//
// Created by aurailus on 22/04/19.
//

#include "Region.h"

Region::Region(glm::vec3 pos) :
    pos(pos) {

    for (int i = 0; i < arrayLength; i++) {
        mapBlocks[i] = nullptr;
    }
}

std::shared_ptr<MapBlock> Region::operator[](int index) {
    return mapBlocks[index];
}

void Region::set(int index, std::shared_ptr<MapBlock> block) {
    mapBlocks[index] = block;
}