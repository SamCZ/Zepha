//
// Created by aurailus on 04/04/19.
//

#ifndef ZEUS_MAPBLOCK_H
#define ZEUS_MAPBLOCK_H


#include <vec3.hpp>
#include <array>
#include "../helpers/TransPos.h"
#include "../blocks/BlockChunk.h"

template <class C>
class MapBlock {
public:
    explicit MapBlock(glm::vec3 pos);

    C* operator[](int index);
    void set(int index, C* block);

    ~MapBlock();
private:
    glm::vec3 pos {};

    const static int arrayLength = TransPos::MAPBLOCK_SIZE * TransPos::MAPBLOCK_SIZE * TransPos::MAPBLOCK_SIZE;
    std::array<C*, arrayLength> blockChunks;
};

template<class C>
MapBlock<C>::MapBlock(glm::vec3 pos) {
    for (int i = 0; i < arrayLength; i++) {
        blockChunks[i] = nullptr;
    }

    this->pos = pos;
}

template<class C>
C* MapBlock<C>::operator[](int index) {
    return blockChunks[index];
}

template<class C>
void MapBlock<C>::set(int index, C* chunk) {
    blockChunks[index] = chunk;
}

template<class C>
MapBlock<C>::~MapBlock() {
    for (int i = 0; i < arrayLength; i++) {
        delete blockChunks[i];
    }
}

#endif //ZEUS_MAPBLOCK_H
