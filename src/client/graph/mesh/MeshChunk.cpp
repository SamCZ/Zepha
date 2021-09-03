#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "MeshChunk.h"

#include "client/graph/Renderer.h"

MeshChunk::MeshChunk(const vec3 pos, const vec<Vertex>& vertices, const vec<u32>& indices) :
	ChunkRenderElem(pos), mesh(make_unique<Mesh>(vertices, indices)) {}

void MeshChunk::draw(Renderer& renderer) {
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, pos * 16.f);
	renderer.setModelMatrix(model);
	mesh->draw();
}

bool MeshChunk::updateChunkUse(ivec3 pos, bool used) {
	return used;
}

MeshChunk::Mesh::Mesh(const vec<Vertex>& vertices, const vec<u32>& indices) {
	this->indCount = indices.size();

	genArrays(vertices.size() * sizeof(Vertex), indices.size() * sizeof(u32), &vertices.front(), &indices.front());

	u32 idx = 0;
	createVertexAttrib(idx++, 3, GL_FLOAT,         false, STRIDE_OFFSET(Vertex, position));
	createVertexAttrib(idx++, 2, GL_FLOAT,         false, STRIDE_OFFSET(Vertex, texCoords));
	createVertexAttrib(idx++, 3, GL_UNSIGNED_BYTE, true,  STRIDE_OFFSET(Vertex, blendColor));
	createVertexAttrib(idx++, 2, GL_FLOAT,         false, STRIDE_OFFSET(Vertex, blendMaskCoords));
	createVertexAttrib(idx++, 1, GL_FLOAT,         false, STRIDE_OFFSET(Vertex, normal));
	createVertexAttrib(idx++, 4, GL_UNSIGNED_BYTE, true,  STRIDE_OFFSET(Vertex, light));
	createVertexAttrib(idx++, 1, GL_UNSIGNED_BYTE, true,  STRIDE_OFFSET(Vertex, shaderMod));
	createVertexAttrib(idx++, 3, GL_FLOAT        , false, STRIDE_OFFSET(Vertex, modValues));
}