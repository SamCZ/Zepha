#pragma once

#include <functional>

#include "util/Types.h"
#include "util/Interp.h"

class NoiseSample {
public:
	typedef std::function<f32(vec3 pos)> fill_function;
	
	NoiseSample(u16 precision, f32 scaleBy = 1);
	
	NoiseSample(u16vec2 precision, vec2 scaleBy = vec2(1));
	
	void populate(const fill_function& fn);
	
	inline f32 get(vec3 pos) {
		vec3 scaled = pos * vec3(precision) / scaleBy;
		
		ivec3 a = { scaled.x, scaled.y, scaled.z };
		vec3 factor = { scaled.x - a.x, scaled.y - a.y, scaled.z - a.z };
		ivec3 b = {
			(std::min)(static_cast<i32>(std::ceil(scaled.x)), precision.x),
			(std::min)(static_cast<i32>(std::ceil(scaled.y)), precision.y),
			(std::min)(static_cast<i32>(std::ceil(scaled.z)), precision.z) };

		assert(index(b.x, b.y, b.z) < data.size());
		
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
	inline u16 index(u16 x, u16 y, u16 z) {
		return x * (precision.x + 1) * (precision.y + 1) + y * (precision.x + 1) + z;
	};
	
	std::vector<f32> data {};
	ivec3 precision {};
	vec3 scaleBy;
};

