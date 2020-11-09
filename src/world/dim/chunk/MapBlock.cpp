//
// Created by aurailus on 22/04/19.
//

#include "MapBlock.h"

#include "world/dim/chunk/Chunk.h"
#include "util/net/Deserializer.h"

MapBlock::MapBlock(glm::ivec3 pos) :
	pos(pos) {
	for (unsigned short i = 0; i < 64; i++) chunks[i] = nullptr;
}

std::shared_ptr<Chunk> MapBlock::get(unsigned short index) const {
	auto _ = getReadLock();
	return chunks[index];
}

void MapBlock::set(unsigned short index, std::shared_ptr<Chunk> chunk) {
	auto _ = getWriteLock();
	if (chunks[index] == nullptr) count++;
	chunks[index] = chunk;
}

void MapBlock::remove(unsigned short index) {
	auto _ = getWriteLock();
	if (chunks[index] != nullptr) count--;
	chunks[index] = nullptr;
}