//
// Created by aurailus on 15/02/19.
//

#pragma once

#include <vector>
#include <functional>
#include <glm/glm.hpp>

#include "util/Interp.h"

class NoiseSample {
	public:
	typedef std::function<float(glm::vec3 pos)> fill_function;
	
	NoiseSample(unsigned int precision, float scaleBy = 1);
	
	NoiseSample(glm::ivec2 precision, glm::vec2 scaleBy = { 1, 1 });
	
	void populate(const fill_function& fn);
	
	inline float get(glm::vec3 pos) {
		glm::vec3 scaled = pos * glm::vec3(precision) / scaleBy;
		
		glm::ivec3 a = { scaled.x, scaled.y, scaled.z };
		glm::vec3 factor = { scaled.x - a.x, scaled.y - a.y, scaled.z - a.z };
		glm::ivec3 b = { NoiseSample::min(int(std::ceil(scaled.x)), precision.x),
			NoiseSample::min(int(std::ceil(scaled.y)), precision.y),
			NoiseSample::min(int(std::ceil(scaled.z)), precision.z) };

//        assert(a.x + factor.x <= precision.x && a.y + factor.y <= precision.y && a.z + factor.z <= precision.z);
		
		// No vertical interpolation
		if (precision.y == 0)
			return Interp::bilerp(
				data[index(a.x, a.y, a.z)], data[index(b.x, a.y, a.z)],
				data[index(a.x, a.y, b.z)], data[index(b.x, a.y, b.z)],
				factor.x, factor.z);
		
		return Interp::trilerp(
			data[index(a.x, a.y, a.z)], data[index(b.x, a.y, a.z)],
			data[index(a.x, a.y, b.z)], data[index(b.x, a.y, b.z)],
			data[index(a.x, b.y, a.z)], data[index(b.x, b.y, a.z)],
			data[index(a.x, b.y, b.z)], data[index(b.x, b.y, b.z)],
			factor.x, factor.z, factor.y);
	}
	
	private:
	inline unsigned int index(int x, int y, int z) {
		return x * (precision.x + 1) * (precision.y + 1) + y * (precision.x + 1) + z;
	};
	
	static inline int min(int a, int b) {
		return (a < b ? a : b);
	}
	
	std::vector<float> data{};
	glm::ivec3 precision{};
	glm::vec3 scaleBy;
};

