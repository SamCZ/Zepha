//
// Created by aurailus on 22/04/19.
//

#include "Region.h"

Region::Region(glm::vec3 pos) {
    for (int i = 0; i < arrayLength; i++) {
        mapBlocks[i] = nullptr;
    }

    this->pos = pos;
}

MapBlock *Region::operator[](int index) {
    return mapBlocks[index];
}

void Region::set(int index, MapBlock *block) {
    mapBlocks[index] = block;
}

Region::~Region() {
    for (int i = 0; i < arrayLength; i++) {
        delete mapBlocks[i];
    }
}