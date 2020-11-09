//
// Created by aurailus on 15/12/18.
//

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MeshChunk.h"

#include "client/graph/Renderer.h"
#include "client/graph/mesh/ChunkMesh.h"

void MeshChunk::create(std::vector<ChunkVertex>& vertices, std::vector<unsigned int>& indices) {
	this->mesh = std::make_shared<ChunkMesh>();
	mesh->create(vertices, indices);
}

void MeshChunk::draw(Renderer& renderer) {
	glm::mat4 model = glm::mat4(1.0);
	model = glm::translate(model, pos * static_cast<float>(16));
	renderer.setModelMatrix(model);
	mesh->draw();
}

void MeshChunk::setPos(glm::vec3 pos) {
	this->pos = pos;
}

glm::vec3 MeshChunk::getPos() {
	return pos;
}

bool MeshChunk::updateChunkUse(glm::vec3 pos, bool used) {
	return used;
}