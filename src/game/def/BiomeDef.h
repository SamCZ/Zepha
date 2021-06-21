//
// Stores properties, blocks, and noise generation
// for a biome defined using the Lua API.
// Created by aurailus on 2019-11-13.
//

#pragma once

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <libnoise/module/modulebase.h>

class Structure;

struct BiomeDef {
	std::string identifier = "";
	unsigned int index = 0;
	std::unordered_map<std::string, unsigned short> tags{};
	
	float temperature = 0;
	float humidity = 0;
	float roughness = 0;
	
	unsigned int topBlock = 0;
	unsigned int soilBlock = 0;
	unsigned int rockBlock = 0;
	
	std::vector<noise::module::Module*> heightmap;
	std::vector<noise::module::Module*> volume;
	
	std::vector<std::shared_ptr<Structure>> schematics;
	
	glm::vec3 tint{};
};
