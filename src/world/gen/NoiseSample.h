#pragma once

#include <functional>
#include <FastNoise/FastNoise.h>

#include "util/Types.h"
#include "util/Interp.h"

/**
 * Stores a 2d or 3d array of noise data that can be accessed by local position or index.
 * Noise will only be sampled every `precision` blocks, and interpolated between them.
 * Generates 1 block larger on each access due to interpolation, but the index signature
 * will access it as though it was exactly `size` blocks wide.
 */

class NoiseSample {
public:
	NoiseSample(u16vec2 size, u16 precision);
	NoiseSample(u16vec3 size, u16 precision);
	
	/** Populates the data with a noise generator. */
	void generate(ivec3 pos, const FastNoise::SmartNode<>& generator);
	
	/** Populates the data with a fill function. */
	void fill(const std::function<f32(ivec3)>& fill);
	
	/** Retrieves the value from a local position. */
	inline f32 operator[](u16vec3 pos) {
		return data[index(pos)];
	}
	
	/** Retrieves the value from an index into a `size`-sized cuboid. */
	inline f32 operator[](u32 ind) {
		return data[shiftIndexByOne(ind)];
	}
	
private:
	/** Interpolates the data between sampled points. */
	void interpolateData();
	
	/** Retrieves the index for a position vector. */
	inline u32 index(u16vec3 pos) {
		return index(pos.x, pos.y, pos.z);
	};
	
	/** Retrieves the index for the inner data from position coordinates. */
	inline u32 index(u16 x, u16 y, u16 z) {
		return static_cast<u32>(x) * (size.x + 1) * (size.y + 1) + y * (size.x + 1) + z;
	};
	
	/** Shifts an index value from the provided `size` to the internal dimensions. */
	inline u32 shiftIndexByOne(u32 ind) {
		u16vec3 vec = {};
		
		let divY = size.y == 0 ? 1 : size.y;
		
		vec.z = ind / (size.x * divY);
		ind -= (vec.z * size.x * divY);
		vec.y = ind / size.x;
		vec.x = ind % size.x;
		
		return index(vec);
	};
	
	/** The specified size of the Sample. The actual size is 1 larger on each axis. */
	u16vec3 size;
	
	/** The precision at which points are sampled. */
	u16 precision;
	
	/** The point data. */
	vec<f32> data {};
};

