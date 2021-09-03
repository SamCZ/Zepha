#include <cmath>

//
// Created by aurailus on 08/04/19.
//

#pragma once

#include "util/Vec.h"

class SelectionBox {
	public:
	SelectionBox() = default;
	
	SelectionBox(glm::vec3 a, glm::vec3 b) : a(a), b(b) {};
	
	SelectionBox& operator=(const SelectionBox& s) = default;
	
	bool operator==(const SelectionBox& o) {
		return (a == o.a && b == o.b);
	}
	
	bool operator!=(const SelectionBox& o) {
		return (a != o.a || b != o.b);
	}
	
	void set(glm::vec3 a, glm::vec3 b) {
		this->a = a;
		this->b = b;
	}
	
	EVec intersects(glm::vec3 vec) {
		const float THRESH = 0.02f;
		
		if (std::abs(vec.y - b.y) < THRESH && vec.x > a.x && vec.x < b.x && vec.z > a.z && vec.z < b.z)
			return EVec::YPOS;
		if (std::abs(vec.y - a.y) < THRESH && vec.x > a.x && vec.x < b.x && vec.z > a.z && vec.z < b.z)
			return EVec::YNEG;
		if (std::abs(vec.z - a.z) < THRESH && vec.x > a.x && vec.x < b.x && vec.y > a.y && vec.y < b.y)
			return EVec::ZNEG;
		if (std::abs(vec.z - b.z) < THRESH && vec.x > a.x && vec.x < b.x && vec.y > a.y && vec.y < b.y)
			return EVec::ZPOS;
		if (std::abs(vec.x - b.x) < THRESH && vec.z > a.z && vec.z < b.z && vec.y > a.y && vec.y < b.y)
			return EVec::XPOS;
		if (std::abs(vec.x - a.x) < THRESH && vec.z > a.z && vec.z < b.z && vec.y > a.y && vec.y < b.y)
			return EVec::XNEG;
		
		return EVec::NONE;
	}
	
	static glm::ivec3 faceToOffset(EVec f) {
		return Vec::TO_VEC[static_cast<unsigned short>(f)];
	}
	
	glm::vec3 a {}, b { 1, 1, 1 };
};

