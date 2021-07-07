//
// Created by aurailus on 23/07/19.
//

#pragma once

#include <vector>
#include <glm/vec3.hpp>

#include "client/graph/mesh/ChunkVertex.h"

struct ChunkMeshDetails {
	std::vector<ChunkVertex> vertices;
	std::vector<unsigned int> indices;
	
	glm::ivec3 pos {};
};
