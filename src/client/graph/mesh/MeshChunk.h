//
// Created by aurailus on 15/12/18.
//

#pragma once

#include <memory>
#include <vector>

#include "ChunkRenderElem.h"
#include "client/graph/Drawable.h"

class ChunkMesh;

class ChunkVertex;

class MeshChunk : public ChunkRenderElem, Drawable {
	public:
	MeshChunk() = default;
	
	void create(std::vector<ChunkVertex>& vertices, std::vector<unsigned int>& indices);
	
	void draw(Renderer& renderer) override;
	
	bool updateChunkUse(glm::vec3 chunk, bool used) override;
	
	void setPos(glm::vec3 pos);
	
	glm::vec3 getPos() override;
	
	private:
	std::shared_ptr<ChunkMesh> mesh = nullptr;
	glm::vec3 pos{};
};