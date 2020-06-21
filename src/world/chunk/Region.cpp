//
// Created by aurailus on 22/04/19.
//

#include "Region.h"

#include "../../util/Space.h"

Region::Region(glm::ivec3 pos) :
    pos(pos) {
    for(unsigned short i = 0; i < 64; i++) mapBlocks[i] = nullptr;
}

std::shared_ptr<MapBlock> Region::operator[](unsigned short index) {
    return mapBlocks[index];
}

void Region::set(unsigned short index, std::shared_ptr<MapBlock> block) {
    if (mapBlocks[index] == nullptr) count++;
    mapBlocks[index] = block;
}

void Region::remove(unsigned short index) {
    if (mapBlocks[index] != nullptr) count--;
    mapBlocks[index] = nullptr;
}
