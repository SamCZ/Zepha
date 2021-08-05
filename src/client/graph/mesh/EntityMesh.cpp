//
// Created by aurailus on 18/06/19.
//

#include "EntityMesh.h"

EntityMesh::EntityMesh(const EntityMesh& o) :
	vertices(o.vertices),
	indices(o.indices) {
	this->indCount = o.indCount;
	if (indCount > 0) initModel();
}

void EntityMesh::create(const std::vector<EntityVertex>& vertices, const std::vector<unsigned int>& indices) {
	indCount = static_cast<GLsizei>(indices.size());
	this->vertices = vertices;
	this->indices = indices;
	
	initModel();
}

void EntityMesh::initModel() {
	if (vertices.size() == 0) return;
	
	genArrays(static_cast<unsigned int>(vertices.size() * sizeof(EntityVertex)),
		static_cast<unsigned int>(indices.size() * sizeof(unsigned int)),
		&vertices.front(), &indices.front());
	
	unsigned int idx = 0;
	createVertexAttrib(idx++, 3, GL_FLOAT, false, STRIDE_OFFSET_ENTITY(position));
	createVertexAttrib(idx++, 4, GL_FLOAT, false, STRIDE_OFFSET_ENTITY(colorData));
	createVertexAttrib(idx++, 3, GL_FLOAT, false, STRIDE_OFFSET_ENTITY(colorBlend));
	createVertexAttrib(idx++, 1, GL_FLOAT, false, STRIDE_OFFSET_ENTITY(useTex));
	createVertexAttrib(idx++, 3, GL_FLOAT, false, STRIDE_OFFSET_ENTITY(normal));
	createVertexAttrib(idx++, 4, GL_INT, true, STRIDE_OFFSET_ENTITY(boneIDs));
	createVertexAttrib(idx, 4, GL_FLOAT, false, STRIDE_OFFSET_ENTITY(boneWeights));
	
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}