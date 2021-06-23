#include "NoiseSample.h"

NoiseSample::NoiseSample(u16 precision, f32 scaleBy) :
	NoiseSample({ precision, precision }, { scaleBy, scaleBy }) {}

NoiseSample::NoiseSample(u16vec2 precision, glm::vec2 scaleBy) :
	precision(precision.x, precision.y, precision.x),
	scaleBy(scaleBy.x, scaleBy.y, scaleBy.x) {
	data.resize((this->precision.x + 1) * (this->precision.y + 1) * (this->precision.z + 1));
}

void NoiseSample::populate(const NoiseSample::fill_function& fn) {
	vec3 pos {}, precisionFloat = vec3(precision);
	for (pos.x = 0; pos.x <= precisionFloat.x; pos.x++)
		for (pos.y = 0; pos.y <= precisionFloat.y; pos.y++)
			for (pos.z = 0; pos.z <= precisionFloat.z; pos.z++) {
				vec3 queryPos = pos / precisionFloat * scaleBy;
				if (queryPos.y == NAN) queryPos.y = 0;
				data[index(pos.x, pos.y, pos.z)] = fn(queryPos);
			}
}
