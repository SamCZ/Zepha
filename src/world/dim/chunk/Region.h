//
// The Region class, contains 64 MapBlocks in a 4^3 cube.
// Created by aurailus on 04/04/19.
//

#pragma once

#include <array>
#include <memory>
#include <glm/vec3.hpp>

#include "util/Lockable.h"

class MapBlock;

class Region {
	public:
	Region(glm::ivec3 pos);
	
	std::shared_ptr<MapBlock> get(unsigned short index) const;
	
	void set(unsigned short index, std::shared_ptr<MapBlock> block);
	
	void remove(unsigned short index);
	
	glm::ivec3 pos{};
	unsigned short count = 0;
	private:
	std::array<std::shared_ptr<MapBlock>, 64> mapBlocks{};
};
