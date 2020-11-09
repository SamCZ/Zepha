//
// Created by aurailus on 24/08/19.
//

#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class EntityVertex {
	public:
	EntityVertex() = default;
	
	EntityVertex(glm::vec3 position, glm::vec4 colorData, glm::vec3 colorBlend, float useTex, glm::vec3 normal,
		glm::ivec4 boneIDs, glm::vec4 boneWeights) : position(position), colorData(colorData),
		colorBlend(colorBlend), useTex(useTex), normal(normal), boneIDs(boneIDs), boneWeights(boneWeights) {};
	
	glm::vec3 position{};
	glm::vec4 colorData{};
	glm::vec3 colorBlend{};
	float useTex = false;
	glm::vec3 normal{};
	glm::ivec4 boneIDs{};
	glm::vec4 boneWeights{};
};

#define STRIDE_OFFSET_ENTITY(m) sizeof(struct EntityVertex), (void *)offsetof(struct EntityVertex, m)
