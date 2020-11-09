//
// Created by aurailus on 2020-07-24.
//

#pragma once

class World;

class LuaParser;

class BiomeAtlas;

class DefinitionAtlas;

class Subgame {
	public:
	virtual DefinitionAtlas& getDefs() = 0;
	
	virtual BiomeAtlas& getBiomes() = 0;
	
	virtual LuaParser& getParser() = 0;
};