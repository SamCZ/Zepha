#pragma once

#include "Mesh.h"

#include "EntityVertex.h"

class EntityMesh : public Mesh {
public:
	EntityMesh() = default;
	
	EntityMesh(const EntityMesh& o);
	
	void create(const vec<EntityVertex>& vertices, const vec<u32>& indices);
	
	~EntityMesh() = default;
	
private:
	void initModel();

	vec<u32> indices {};
	vec<EntityVertex> vertices {};
};
