#pragma once

#include "util/Types.h"

#include <sol/forward.hpp>
#include <FastNoise/FastNoise.h>

namespace NoiseFromLua {
	/** Builds a FastNoise node tree from a Lua table. */
	FastNoise::SmartNode<> parse(sol::table table);
};


