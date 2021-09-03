//
// Created by aurailus on 17/04/19.
//

#pragma once

#include "util/Util.h"

struct AtlasRef {
	u16vec2 pos {};
	u16vec2 size {};
	uvec2 rawPos {};
	uvec2 rawSize {};
	vec4 uv {};
	
	string name = "";
	bool base = false;
};

