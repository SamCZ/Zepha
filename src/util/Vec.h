//
// Created by aurailus on 29/03/19.
//

#pragma once

#include <array>
#include <functional>
#include <glm/vec3.hpp>
#include <unordered_map>

#include "Space.h"

enum class EVec {
	LEFT = 0, RIGHT = 1, BOTTOM = 2, TOP = 3, BACK = 4, FRONT = 5,
	XNEG = 0, XPOS = 1, YNEG = 2, YPOS = 3, ZNEG = 4, ZPOS = 5,
	
	INVALID = -1, NONE = -1,
	NO_CULL = 6,
};

namespace Vec {
	
	// Comparison Functions
	
	struct vec3 {
		size_t operator()(const glm::vec3& k) const {
			return std::hash<float>()(k.x) ^ std::hash<float>()(k.y) ^ std::hash<float>()(k.z);
		}
	};
	
	struct ivec3 {
		size_t operator()(const glm::ivec3& k) const {
			return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z);
		}
	};
	
	struct ivec4 {
		size_t operator()(const glm::ivec4& k) const {
			return std::hash<int>()(k.x) ^ std::hash<int>()(k.y) ^ std::hash<int>()(k.z) ^ std::hash<int>()(k.w);
		}
	};
	
	// Adjacent Arrays & Maps
	
	const static std::array<glm::ivec3, 6> TO_VEC = {
		glm::ivec3 { -1, 0, 0 },
		glm::ivec3 { 1, 0, 0 },
		glm::ivec3 { 0, -1, 0 },
		glm::ivec3 { 0, 1, 0 },
		glm::ivec3 { 0, 0, -1 },
		glm::ivec3 { 0, 0, 1 }
	};
	
	const static std::array<glm::ivec3, 6> TO_VEC_R = {
		glm::ivec3 { 1, 0, 0 },
		glm::ivec3 { -1, 0, 0 },
		glm::ivec3 { 0, 1, 0 },
		glm::ivec3 { 0, -1, 0 },
		glm::ivec3 { 0, 0, 1 },
		glm::ivec3 { 0, 0, -1 }
	};
	
	const static std::unordered_map<glm::ivec3, EVec, Vec::ivec3> TO_ENUM = {
		{ TO_VEC[0], EVec::XNEG }, { TO_VEC[1], EVec::XPOS }, { TO_VEC[2], EVec::YNEG },
		{ TO_VEC[3], EVec::YPOS }, { TO_VEC[4], EVec::ZNEG }, { TO_VEC[5], EVec::ZPOS }};
	
	const static std::unordered_map<glm::ivec3, EVec, Vec::ivec3> TO_ENUM_R = {
		{ TO_VEC[0], EVec::XPOS }, { TO_VEC[1], EVec::XNEG }, { TO_VEC[2], EVec::YPOS },
		{ TO_VEC[3], EVec::YNEG }, { TO_VEC[4], EVec::ZPOS }, { TO_VEC[5], EVec::ZNEG }};
};

