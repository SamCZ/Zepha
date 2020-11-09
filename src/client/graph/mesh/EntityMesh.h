//
// Created by aurailus on 18/06/19.
//

#pragma once

#include <vector>

#include "Mesh.h"

#include "EntityVertex.h"

class EntityMesh : public Mesh {
	public:
	EntityMesh() = default;
	
	EntityMesh(const EntityMesh& o);
	
	void create(const std::vector<EntityVertex>& vertices, const std::vector<unsigned int>& indices);
	
	~EntityMesh() = default;
	
	private:
	void initModel();
	
	std::vector<EntityVertex> vertices{};
	std::vector<unsigned int> indices{};
};
