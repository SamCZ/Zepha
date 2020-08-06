//
// Created by aurailus on 22/04/19.
//

#include <gzip/compress.hpp>
#include <gzip/decompress.hpp>
#include <gzip/utils.hpp>
#include <iostream>

#include "MapBlock.h"

#include "Chunk.h"
#include "../../net/Serializer.h"
#include "../../net/Deserializer.h"
#include "../../util/Util.h"

MapBlock::MapBlock(glm::ivec3 pos) :
    pos(pos) {
    for(unsigned short i = 0; i < 64; i++) chunks[i] = nullptr;
}

std::shared_ptr<Chunk> MapBlock::get(unsigned short index) const {
    auto l = getReadLock();
    return chunks[index];
}

void MapBlock::set(unsigned short index, std::shared_ptr<Chunk> chunk) {
    auto l = getWriteLock();
    if (chunks[index] == nullptr) count++;
    chunks[index] = chunk;
}

void MapBlock::remove(unsigned short index) {
    auto l = getWriteLock();
    if (chunks[index] != nullptr) count--;
    chunks[index] = nullptr;
}