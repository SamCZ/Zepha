#pragma once

#include <vector>
#include <sol/forward.hpp>
#include <libnoise/module/add.h>
#include <libnoise/module/module.h>
#include <libnoise/module/modulebase.h>

namespace NoiseFromLua {
	
	/**
	 * Builds a noise module vector from a Lua noise table.
	 * The top level module will always be the last element in the vector.
	 *
	 * @param noise - The lua noise definition to parse.
	 * @returns a vector containing all of the noise modules built.
	 */
	 
	std::vector<noise::module::Module*> build(sol::table noise);
	
	
	/**
	 * Will initialize Noise::Module instances from a lua noise module definition, and recursively initialize it's
	 * child modules as well. All modules will be added in reverse-discovered-order to the modules vector reference
	 * passed in. The top level module will always be the last element in the vector.
	 *
	 * @param modules - The vector reference to insert generated noise modules into.
	 * @param noise - The lua noise definition to parse.
	 * @returns the noise module that was parsed.
	 */
 
	noise::module::Module* parseNoise(std::vector<noise::module::Module*>& modules, sol::table noise);
};


