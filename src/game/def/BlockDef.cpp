//
// Created by aurailus on 10/06/19.
//

#include "BlockDef.h"

void BlockDef::createModel() {
	std::unique_ptr<EntityMesh> entity = std::make_unique<EntityMesh>();
	unsigned int indOffset = 0;
	
	std::vector<EntityVertex> entityVertices;
	std::vector<unsigned int> indices;
	
	for (std::vector<MeshPart>& pArray : model.parts) {
		for (MeshPart& p : pArray) {
			for (const BlockModelVertex& vertex : p.vertices) {
				entityVertices.push_back(EntityVertex{
					vertex.pos - glm::vec3(0.5f),
					{ vertex.tex.x, vertex.tex.y, 0, 1 },
					{ 1, 1, 1 },
					true,
					vertex.nml,
					{}, {}
				});
			}
			
			for (unsigned int index : p.indices) indices.push_back(indOffset + index);
			indOffset += p.vertices.size();
		}
	}
	
	entity->create(entityVertices, indices);
	entityModel = std::make_shared<Model>();
	entityModel->fromMesh(std::move(entity));
}

bool BlockDef::hasInteraction() {
	return callbacks.count(Callback::INTERACT) || callbacks.count(Callback::INTERACT_CLIENT);
}
