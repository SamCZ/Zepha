//
// Created by aurailus on 2020-02-01.
//

#pragma once

#include <vector>
#include <string>
#include <glm/vec3.hpp>

class DefinitionAtlas;

struct Structure {
	glm::ivec3 origin {};
	glm::ivec3 dimensions {};
	std::vector<unsigned int> layout {};
	
	float probability = 0;
	
	inline unsigned int length() { return layout.size(); }
	
	glm::ivec3 getOffset(unsigned int ind);
	
	unsigned int index(const glm::ivec3& vec);
};
