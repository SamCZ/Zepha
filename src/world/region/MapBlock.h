//
// Created by aurailus on 18/04/19.
//

#ifndef ZEUS_MAPBLOCK_H
#define ZEUS_MAPBLOCK_H

#include <vec3.hpp>
#include <array>
#include <memory>
#include "../../util/TransPos.h"
#include "../chunk/BlockChunk.h"

template <class C>
class MapBlock {
public:
    explicit MapBlock(glm::vec3 pos);

    std::shared_ptr<C> operator[](int index);
    void set(int index, std::shared_ptr<C> chunk);

    ~MapBlock() = default;
private:
    glm::vec3 pos {};

    const static int arrayLength = TransPos::MAPBLOCK_SIZE * TransPos::MAPBLOCK_SIZE * TransPos::MAPBLOCK_SIZE;
    std::array<std::shared_ptr<C>, arrayLength> blockChunks;
};

template<class C>
MapBlock<C>::MapBlock(glm::vec3 pos) {
    for (int i = 0; i < arrayLength; i++) {
        blockChunks[i] = nullptr;
    }

    this->pos = pos;
}

template<class C>
std::shared_ptr<C> MapBlock<C>::operator[](int index) {
    return blockChunks[index];
}

template<class C>
void MapBlock<C>::set(int index, std::shared_ptr<C> chunk) {
    blockChunks[index] = chunk;
}

#endif //ZEUS_MAPBLOCK_H
