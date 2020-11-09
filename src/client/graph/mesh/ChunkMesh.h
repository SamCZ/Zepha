//
// Created by aurailus on 25/11/18.
//

#pragma once

#include <vector>
#include <stdexcept>

#include "Mesh.h"

class ChunkVertex;

class ChunkMesh : public Mesh {
	public:
	ChunkMesh() = default;
	
	ChunkMesh(const ChunkMesh& o) { throw std::runtime_error("No copy constructor for ChunkMeshes"); };
	
	void create(const std::vector<ChunkVertex>& vertices, const std::vector<unsigned int>& indices);
	
	~ChunkMesh() = default;
};
