#pragma once

#include <functional>
#include <FastNoise/FastNoise.h>

#include "util/Types.h"
#include "util/Interp.h"

class NoiseSample {
public:
	typedef std::function<f32(ivec3 pos)> fill_function;
	
	NoiseSample(u16vec2 size, u16 precision);
	NoiseSample(u16vec3 size, u16 precision);
	
	void generate(ivec3 pos, const FastNoise::SmartNode<>& generator);
	
	void fill(const fill_function& fill);
	
	inline f32 operator[](u16vec3 pos) {
		return data[index(pos)];
	}
	
	inline f32 operator[](u32 ind) {
		return data[shiftIndexByOne(ind)];
	}
	
//	inline f32 get(vec3 pos) {
//		vec3 scaled = pos * vec3(precision) / scaleBy;
//
//		ivec3 a = { scaled.x, scaled.y, scaled.z };
//		vec3 factor = { scaled.x - a.x, scaled.y - a.y, scaled.z - a.z };
//		ivec3 b = {
//			(std::min)(static_cast<i32>(std::ceil(scaled.x)), precision.x),
//			(std::min)(static_cast<i32>(std::ceil(scaled.y)), precision.y),
//			(std::min)(static_cast<i32>(std::ceil(scaled.z)), precision.z) };
//
//		assert(index(b.x, b.y, b.z) < data.size());
//
//		// No vertical interpolation
//		if (precision.y == 0)
//			return Interp::bilerp(
//				data[index(a.x, a.y, a.z)], data[index(b.x, a.y, a.z)],
//				data[index(a.x, a.y, b.z)], data[index(b.x, a.y, b.z)],
//				factor.x, factor.z);
//
//		return Interp::trilerp(
//			data[index(a.x, a.y, a.z)], data[index(b.x, a.y, a.z)],
//			data[index(a.x, a.y, b.z)], data[index(b.x, a.y, b.z)],
//			data[index(a.x, b.y, a.z)], data[index(b.x, b.y, a.z)],
//			data[index(a.x, b.y, b.z)], data[index(b.x, b.y, b.z)],
//			factor.x, factor.z, factor.y);
//	}
	
private:
	void interpolateData();
	
//	inline u32 innerIndex(u16 x, u16 y, u16 z) {
//		return static_cast<u32>(x) * size.x * size.y + y * size.x + z;
//	};
	
	inline u32 index(u16vec3 pos) {
		return index(pos.x, pos.y, pos.z);
	};
	
	inline u32 index(u16 x, u16 y, u16 z) {
		return static_cast<u32>(x) * (size.x + 1) * (size.y + 1) + y * (size.x + 1) + z;
	};
	
	inline u32 shiftIndexByOne(u32 ind) {
		u16vec3 vec = {};
		
		let divY = size.y == 0 ? 1 : size.y;
		
		vec.z = ind / (size.x * divY);
		ind -= (vec.z * size.x * divY);
		vec.y = ind / size.x;
		vec.x = ind % size.x;
		
		return index(vec);
	};
	
	u16vec3 size;
	u16 precision;
	vec<f32> data {};
};

