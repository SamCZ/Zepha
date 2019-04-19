//
// Created by aurailus on 04/04/19.
//

#ifndef ZEUS_REGION_H
#define ZEUS_REGION_H


#include <vec3.hpp>
#include <array>
#include "MapBlock.h"
#include "../../util/TransPos.h"

template <class C>
class Region {
public:
    explicit Region(glm::vec3 pos);

    MapBlock<C>* operator[](int index);
    void set(int index, MapBlock<C>* block);

    ~Region();
private:
    glm::vec3 pos {};

    const static int arrayLength = TransPos::REGION_SIZE * TransPos::REGION_SIZE * TransPos::REGION_SIZE;
    std::array<MapBlock<C>*, arrayLength> mapBlocks;
};

template<class C>
Region<C>::Region(glm::vec3 pos) {
    for (int i = 0; i < arrayLength; i++) {
        mapBlocks[i] = nullptr;
    }

    this->pos = pos;
}

template<class C>
MapBlock<C> *Region<C>::operator[](int index) {
    return mapBlocks[index];
}

template<class C>
void Region<C>::set(int index, MapBlock<C> *block) {
    mapBlocks[index] = block;
}

template<class C>
Region<C>::~Region() {
    for (int i = 0; i < arrayLength; i++) {
        delete mapBlocks[i];
    }
}


#endif //ZEUS_REGION_H
