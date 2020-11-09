//
// Created by aurailus on 30/12/18.
//

#pragma once

#include <cmath>
#include <glm/vec3.hpp>

class Player;

class Ray {
	public:
	Ray() = default;
	
	explicit Ray(Player& player);
	
	Ray(glm::vec3 pos, float yaw, float pitch);
	
	void step(float scale);
	
	glm::vec3 getEnd();
	
	float getLength();
	
	private:
	glm::vec3 start;
	glm::vec3 pos;
	glm::vec3 dir;
};

