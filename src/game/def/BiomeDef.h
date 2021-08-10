//
// Stores properties, blocks, and noise generation
// for a biome defined using the Lua API.
// Created by aurailus on 2019-11-13.
//

#pragma once

#include <FastNoise/FastNoise.h>

class Structure;

struct BiomeDef {
	u32 index = 0;
	string identifier = "";
	std::unordered_map<string, u16> tags {};
	
	f32 temperature = 0;
	f32 humidity = 0;
	f32 roughness = 0;
	
	u16 topBlock = 0;
	u16 soilBlock = 0;
	u16 rockBlock = 0;
	
	FastNoise::SmartNode<> heightmap;
	FastNoise::SmartNode<> volume;
	
	vec<sptr<Structure>> schematics;
	
	vec3 tint {};
};
