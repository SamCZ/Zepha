//
// Created by aurailus on 22/04/19.
//

#include "Region.h"

Region::Region(glm::vec3 pos, glm::vec3 rawPos) :
    pos(pos),
    rawPos(rawPos) {

    for (int i = 0; i < arrayLength; i++) {
        mapBlocks[i] = nullptr;
    }
}

MapBlock *Region::operator[](int index) {
    return mapBlocks[index];
}

void Region::set(int index, MapBlock *block) {
    mapBlocks[index] = block;
}

glm::vec3 Region::getRawPos() {
    return rawPos;
}

Region::~Region() {
    for (int i = 0; i < arrayLength; i++) {
        delete mapBlocks[i];
    }
}
