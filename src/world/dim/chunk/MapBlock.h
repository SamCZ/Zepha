//
// The MapBlock class, contains 64 Chunks in a 4^3 cube.
// Created by aurailus on 18/04/19.
//

#pragma once

#include <array>
#include <memory>
#include <vector>
#include <glm/vec3.hpp>

#include "util/Lockable.h"

class Chunk;

class Deserializer;

class MapBlock {
public:
	MapBlock(glm::ivec3 pos);
	
	MapBlock(const MapBlock& o);
	
	std::shared_ptr<Chunk> get(unsigned short index) const;
	
	void set(unsigned short index, std::shared_ptr<Chunk> chunk);
	
	void remove(unsigned short index);
	
	glm::ivec3 pos {};
	bool generated = false;
	unsigned short count = 0;

private:
	std::array<std::shared_ptr<Chunk>, 64> chunks;
};
