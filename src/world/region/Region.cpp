//
// Created by aurailus on 22/04/19.
//

#include "Region.h"

Region::Region(glm::ivec3 pos) :
    pos(pos) {

    for (unsigned short i = 0; i < 64; i++) {
        mapBlocks[i] = nullptr;
    }
}

std::shared_ptr<MapBlock> Region::operator[](unsigned short index) {
    return mapBlocks[index];
}

void Region::set(unsigned short index, std::shared_ptr<MapBlock> block) {
    mapBlocks[index] = block;
    count++;
}

void Region::remove(unsigned short index) {
    mapBlocks[index] = nullptr;
    count--;
}