#pragma once

class World;
class LuaParser;
class BiomeAtlas;
class DefinitionAtlas;

/**
 * An abstract class representing a subgame, which has
 * methods to access block definitions, biomes, and the lua parser.
 */
 
class Subgame {
public:
	
	/** Gets the definition atlas. */
	virtual DefinitionAtlas& getDefs() = 0;
	
	/** Gets the biome atlas. */
	virtual BiomeAtlas& getBiomes() = 0;
	
	/** Gets the lua parser. */
	virtual LuaParser& getParser() = 0;
};