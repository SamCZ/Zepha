#pragma once

#include "util/Types.h"

struct BlockModelVertex {
	vec3 pos;
	vec3 nml;
	vec2 tex;
	vec2 blendMask;
	vec2 texUVs;
	vec2 blendMaskUVs;
};
