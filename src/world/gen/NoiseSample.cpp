#include <iostream>
#include <util/Util.h>

#include "NoiseSample.h"


NoiseSample::NoiseSample(u16vec2 size, u16 precision) : NoiseSample({ size.x, 0, size.y }, precision) {}

NoiseSample::NoiseSample(u16vec3 size, u16 precision):
	size(size), precision(precision), data((size.x + 1) * (size.y + 1) * (size.z + 1), 0) {
	if (size.x % precision != 0 || (size.y != 0 && size.y % precision) != 0 || size.z % precision != 0)
		throw std::runtime_error("NoiseSample precision must be a divisor of size." +
			Util::toString(size) + " : " + Util::toString(precision));
}

void NoiseSample::generate(ivec3 pos, const FastNoise::SmartNode<>& generator) {
	for (u16 x = 0; x < size.x + 1; x += precision)
		for (u16 y = 0; y < size.y + 1; y += precision)
			for (u16 z = 0; z < size.z + 1; z += precision)
				data[index(x, y, z)] = generator->GenSingle3D(
					pos.x + x, pos.y + y, pos.z + z, 1337);
			
	interpolateData();
}

void NoiseSample::fill(const NoiseSample::fill_function& fill) {
	u16vec3 pos {};
	for (pos.x = 0; pos.x < size.x + 1; pos.x += precision)
		for (pos.y = 0; pos.y < size.y + 1; pos.y += precision)
			for (pos.z = 0; pos.z < size.z + 1; pos.z += precision)
				data[index(pos)] = fill(pos);
	
	interpolateData();
}

void NoiseSample::interpolateData() {
	u16vec3 pos = {};
	for (pos.x = 0; pos.x < size.x + 1; pos.x += 1) {
		for (pos.y = 0; pos.y < size.y + 1; pos.y += 1) {
			for (pos.z = 0; pos.z < size.z + 1; pos.z += 1) {
				if (pos.x % precision == 0 && pos.y % precision == 0 && pos.z % precision == 0) continue;
				
				vec3 frac = vec3(pos) / static_cast<f32>(precision);
				u16vec3 a = u16vec3(glm::floor(frac)) * static_cast<u16>(precision);
				u16vec3 b = u16vec3(glm::ceil(frac)) * static_cast<u16>(precision);
				vec3 factor = frac - glm::floor(frac);
				
				data[index(pos)] = Interp::trilerp(
					data[index(a.x, a.y, a.z)], data[index(b.x, a.y, a.z)],
					data[index(a.x, a.y, b.z)], data[index(b.x, a.y, b.z)],
					data[index(a.x, b.y, a.z)], data[index(b.x, b.y, a.z)],
					data[index(a.x, b.y, b.z)], data[index(b.x, b.y, b.z)],
					factor.x, factor.z, factor.y);
			}
		}
	}
}