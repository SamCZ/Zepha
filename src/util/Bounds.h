/*
 * Helper class containing two points representing a bounding box.
 *
 * - Auri, 07/11/20
 */


#pragma once

#include <glm/vec3.hpp>

class Bounds {
	public:
	Bounds() = default;
	
	Bounds(glm::vec3 a, glm::vec3 b);
	
	glm::vec3 getA();
	
	glm::vec3 getB();
	
	void setA(glm::vec3 vecA);
	
	void setB(glm::vec3 vecB);
	
	bool intersects(glm::vec3 test);
	
	private:
	glm::vec3 a, b;
};
